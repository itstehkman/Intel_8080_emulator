//
//  EmulatorView.m
//  8080_emulator
//
//  Created by Kareem Moussa on 7/9/16.
//  Copyright © 2016 Kareem Moussa. All rights reserved.
//

#import "SpaceInvadersView.h"
#import "structs_and_consts.h"

#define SECONDS_PER_GRAPHICS_CYCLE .017
#define SECONDS_PER_CPU_CYCLE .0000005

static const unsigned int frameBufferPointer = 0x2400;
static const unsigned int RGB_ON = 0xFFFFFFFFL;
static const unsigned int RGB_OFF = 0x00000000L;

@implementation SpaceInvadersView {
    cpu_state _state;
    uint8_t *_frameBufferForView;
    CGContextRef _bitmapContext;
}

- (void)awakeFromNib {
    
    NSString *invadersPath = [[NSBundle mainBundle] pathForResource:@"invaders.rom" ofType:nil];
    [self initFrameBuffer];
    _state = cpu_state_from_rom_file([invadersPath cStringUsingEncoding:NSUTF8StringEncoding]);
    
    NSTimer *graphicsRefreshTimer = [NSTimer timerWithTimeInterval:SECONDS_PER_GRAPHICS_CYCLE target:self selector:@selector(refreshView:) userInfo:nil repeats:YES];
    NSRunLoop *mainThreadRunLoop = [NSRunLoop currentRunLoop];
    // Run graphics in Main Thread
    [mainThreadRunLoop addTimer:graphicsRefreshTimer forMode:NSDefaultRunLoopMode];
    
    // Run CPU on any thread other than main thread.
    dispatch_queue_t backgroundQueueForCPUSerial = dispatch_queue_create("com.8080Emulator.backgroundQueueForCPUSerial", NULL);
    
    dispatch_async(backgroundQueueForCPUSerial, ^{
        
        run_cpu(&_state);
    });
}

- (void)initFrameBuffer {
    _frameBufferForView = calloc(1, 4 * SCREEN_WIDTH * SCREEN_HEIGHT);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    _bitmapContext = CGBitmapContextCreate(_frameBufferForView, SCREEN_WIDTH, SCREEN_HEIGHT, 8, SCREEN_WIDTH * 4,
                                           colorSpace, kCGImageAlphaNoneSkipFirst);
}

- (void)drawRect:(NSRect)dirtyRect {
    int i, j;
    
    //Translate the 1-bit space invaders frame buffer into
    // my 32bpp RGB bitmap.  We have to rotate and
    // flip the image as we go.
    //
    unsigned char *b = (unsigned char *)_frameBufferForView;
    unsigned char *fb = &_state.memory[frameBufferPointer];
    for (i=0; i< 224; i++)
    {
        for (j = 0; j < 256; j+= 8)
        {
            int p;
            //Read the first 1-bit pixel
            // divide by 8 because there are 8 pixels
            // in a byte
            unsigned char pix = fb[(i*(256/8)) + j/8];
            
            //That makes 8 output vertical pixels
            // we need to do a vertical flip
            // so j needs to start at the last line
            // and advance backward through the buffer
            int offset = (255-j)*(224*4) + (i*4);
            unsigned int*p1 = (unsigned int*)(&b[offset]);
            for (p=0; p<8; p++)
            {
                if ( 0!= (pix & (1<<p)))
                    *p1 = RGB_ON;
                else
                    *p1 = RGB_OFF;
                p1-=224;  //next line
            }
        }
    }
    
    
    CGContextRef myContext = [[NSGraphicsContext currentContext] graphicsPort];
    CGImageRef ir = CGBitmapContextCreateImage(_bitmapContext);
    CGContextDrawImage(myContext, self.bounds, ir);
    CGImageRelease(ir);
}

- (void)refreshView:(NSTimer *)timer {
    [self setNeedsDisplay:YES];
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

- (void)keyDown:(NSEvent *)event {
    unichar keyPressed = [[event characters] characterAtIndex:0];
    
    switch (keyPressed) {
        case NSLeftArrowFunctionKey:
        case 'a':
            _state.ports[1] |= 0x20;
            break;
            
        case NSRightArrowFunctionKey:
        case 's':
            _state.ports[1] |= 0x40;
            break;
            
        case ' ':
            _state.ports[1] |= 0x10;
            break;
            
        case 'c':
            _state.ports[1] |= 0x01;
            break;
            
        case '1':
            _state.ports[1] |= 0x04;
            break;
    }
}

- (void)keyUp:(NSEvent *)event {
    unichar keyPressed = [[event characters] characterAtIndex:0];
    
    switch (keyPressed) {
        case NSLeftArrowFunctionKey:
        case 'a':
            _state.ports[1] &= ~0x20;
            break;
            
        case NSRightArrowFunctionKey:
        case 's':
            _state.ports[1] &= ~0x40;
            break;
            
        case ' ':
            _state.ports[1] &= ~0x10;
            break;
            
        case 'c':
            _state.ports[1] &= ~0x01;
            break;
            
        case '1':
            _state.ports[1] &= ~0x04;
            break;
    }
}


@end
