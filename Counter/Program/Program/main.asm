.INCLUDE "M32DEF.INC"

.ORG $0000
RJMP START

.ORG $0016
RJMP TIM0_OVF_ISR

START:

#define MODES_SIZE 4
#define TIME_LENGTH 10
#define YELLOW_LIGHT 3
#define COUNTER_MAX 250
#define TCNT0_VALUE $C1
#define TCCR0_VALUE $03
#define STACK $0200

.DEF RIGHT_DIGIT = R19
.DEF LEFT_DIGIT = R20
.DEF TIME = R21
.DEF MODE = R22
.DEF COUNTER = R23
.DEF PARAM1 = R24
.DEF PARAM2 = R25

TABLE:
.DB $3F, $06, $5B, $4F, $66, $6D, $7D, $27, $7F, $6F

MODES:
.DB $09, $0A, $24, $14

; Iitialize stack
LDI R16, HIGH(STACK)
OUT SPH, R16
LDI R16, LOW(STACK)
OUT SPL, R16

; Initialize ports
LDI R16, $FF
OUT DDRC, R16
OUT DDRD, R16
OUT PORTD, R16
LDI R16, $FF
OUT DDRA, R16

CLR MODE
LDI TIME, TIME_LENGTH
CLR COUNTER

; Initialize timer0
LDI R16, TCNT0_VALUE
OUT TCNT0, R16
LDI R16, $01
OUT TIMSK, R16 ; Enable timer0 interrupt
LDI R16, TCCR0_VALUE
OUT TCCR0, R16 ; Enable timer0, Mode noraml, CLK/8 prescaler
SEI

LOOP:
CALL SEPARATE_DIGITS
CALL DISPLAY_DIGITS
CALL DISPLAY_LIGHTS
RJMP LOOP

SEPARATE_DIGITS:
CLR RIGHT_DIGIT
CLR LEFT_DIGIT
MOV R17, TIME
L1:
CPI R17, $0A
BRLO L2
INC LEFT_DIGIT
SUBI R17, $0A
RJMP L1
L2:
MOV RIGHT_DIGIT, R17
RET

DISPLAY_DIGITS:
LDI ZH, HIGH(TABLE << 1)
LDI PARAM1, $01
MOV PARAM2, RIGHT_DIGIT
CALL DISPLAY_DIGIT
LDI PARAM1, $02
MOV PARAM2, LEFT_DIGIT
CALL DISPLAY_DIGIT
RET

DISPLAY_DIGIT:
OUT PORTD, PARAM1
LDI ZL, LOW(TABLE << 1)
ADD ZL, PARAM2
LPM R16, Z
OUT PORTC, R16
CALL DELAY_2MS
RET

DISPLAY_LIGHTS:
LDI ZH, HIGH(MODES << 1)
LDI ZL, LOW(MODES << 1)
ADD ZL, MODE
LPM R16, Z
OUT PORTA, R16
RET

TIM0_OVF_ISR:
CLR R16
OUT TCCR0, R16 ; Disable timer0
CALL ROUTE
LDI R16, TCNT0_VALUE
OUT TCNT0, R16
LDI R16, TCCR0_VALUE
OUT TCCR0, R16 ; Enable timer0, Mode noraml, CLK/8 prescaler
RETI

ROUTE:
INC COUNTER
LDI R16, COUNTER_MAX
CPSE COUNTER, R16
RET
CLR COUNTER
DEC TIME
CPI TIME, YELLOW_LIGHT
BRNE L3
CALL NEXT_MODE
RET
L3:
LDI R16, $FF ; R16 = -1
CPSE TIME, R16
RET
LDI TIME, TIME_LENGTH
CALL NEXT_MODE
RET

NEXT_MODE:
INC MODE
LDI R16, MODES_SIZE
CPSE MODE, R16
RET
CLR MODE
RET

DELAY_2MS:
LDI R17, $03
LDI R18, $98
L4:
DEC R18
BRNE L4
DEC R17
BRNE L4
RET
