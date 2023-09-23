<#if HarmonyCore.SELECT_RTOS == "BareMetal">
    /* Call the IEEE_802154_MAC Task Handler function */
    WPAN_Task();
<#else>
<#if HarmonyCore.SELECT_RTOS == "FreeRTOS">

    /* Create FreeRTOS task for IEEE_802154_MAC */
     (void)xTaskCreate((TaskFunction_t) _WPAN_Tasks,
                "WPAN_Tasks",
                1024,
                NULL,
                2,
                &xWPAN_Tasks);
</#if>
</#if>