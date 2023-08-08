<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">

/* Handle for the MAC_Tasks. */
TaskHandle_t xWPAN_Tasks;

static void _WPAN_Tasks(  void *pvParameters  )
{     
    while(true)
    {
        WPAN_Tasks();
    }
}

</#if>
