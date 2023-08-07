#ifndef CONSOLE_H_
#define CONSOLE_H_

typedef enum EN_cursorState_t
{
	HIDE, SHOW
}EN_cursorState_t;

typedef enum EN_consoleError_t
{
	CONSOLE_OK, NO_STRING
}EN_consoleError_t;

/* Functions' Prototypes */
EN_consoleError_t systemPrintOut(uint8_t* string);
EN_consoleError_t systemDeleteLine(uint8_t* string);

#endif /* CONSOLE_H_ */