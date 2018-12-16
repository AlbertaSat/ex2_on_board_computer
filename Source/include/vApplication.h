
extern void vAssertCalled( unsigned long ulLine, const char * const pcFileName );

#if(  configCHECK_FOR_STACK_OVERFLOW > 0 )
	extern void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );
#endif

#if( configUSE_TICK_HOOK > 0 )
	extern void vApplicationTickHook( void );
#endif

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
	extern void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );
#endif

