<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">

/* Handle for the MAC_Tasks. */
TaskHandle_t xWPAN_Tasks;

void _WPAN_Tasks(  void *pvParameters  )
{     
    while(1)
    {
        WPAN_Tasks();
    }
}

</#if>
