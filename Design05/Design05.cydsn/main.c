/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

#include "mprintf.h"




/************************************************************************************************/

/* 双向命令, 命令和响应共用 CMD, 如下. */

#define  CMD_GET_STATE      0xE1                    /* CYBLE_STATE_T */

#define  CMD_STRT_SCAN      0xA5                    /* 响应, 1 byte, 0 成功, 其他表示失败. */
#define  CMD_STOP_SCAN      0x5A                    /* 响应, 1 byte, 0 成功, 其他表示失败. */

#define  CMD_CONNECT        0x3C                    /* 响应, 1 byte, 0 成功, 其他表示失败. */
#define  CMD_DISCONNT       0xC3                    /* 响应, 1 byte, 0 成功, 其他表示失败. */

#define  CMD_ENA_NOTY       0xD2                    /* 命令, 2 byte, attr desc handle; 响应, 1byte , 0 成功, 其他表示失败. */
#define  CMD_WRT_NRSP       0x2D                    /* 命令, 2 + var byte, attr handle + var byte; 响应, 1byte , 0 成功, 其他表示失败. */


/* 单向 event 上报. */
#define  CMD_EVT_PEER       0x87                    /* SCANING 状态下, 上报扫描到的 peer 地址. */
#define  CMD_EVT_NOTY       0x78                    /* CONNECT 状态下, 上报的 notify 数据信息. */
#define  CMD_EVT_DISC       0x69                    /* CONNECT 状态下, 上报 peer 主动断开连接. */


/************************************************************************************************/


void  SendPacket( uint8 cmd, uint8 tlen, uint8 * pdat )
{
    int  i;
    
    UART_UartPutChar( 0xAA );
    UART_UartPutChar( (uint32)cmd  );
    UART_UartPutChar( (uint32)tlen  );
    
    for ( i=0; i<tlen; i++ )
    {
        UART_UartPutChar( (uint32)(pdat[i]) );
    }
    
    return;
}



/* 
0: init
1: error
2: stack on.

3: scaning
*/


uint8  g_cur_cmd = 0;
uint32  g_tstamp = 0;
uint32 g_stacks = 0;

/**/
void AppCallBack(uint32 event, void *eventParam)
{
    uint8  tary[64];
    CYBLE_GAPC_ADV_REPORT_T	* advReport;
    CYBLE_GATTC_HANDLE_VALUE_NTF_PARAM_T * notify;
    CYBLE_GAP_CONN_PARAM_UPDATED_IN_CONTROLLER_T * conrsp;
    CYBLE_TO_REASON_CODE_T * preason;
    
    switch (event)
    {
        case CYBLE_EVT_STACK_ON:
            g_stacks = 2;
            break;
        
        case CYBLE_EVT_GAPC_SCAN_START_STOP:
            if ( g_cur_cmd == CMD_STRT_SCAN )
            {
                tary[0] = *(uint8_t *)eventParam;
                SendPacket( CMD_STRT_SCAN, 1, tary );
            }
            else
            {
                tary[0] = *(uint8_t *)eventParam;
                SendPacket( CMD_STOP_SCAN, 1, tary );
            }
            // mprintf( "scan action : %d \n", *(uint8_t *)eventParam );
            break;
        
        case CYBLE_EVT_GAPC_SCAN_PROGRESS_RESULT:
            advReport = (CYBLE_GAPC_ADV_REPORT_T *) eventParam;
            if( advReport->eventType == CYBLE_GAPC_SCAN_RSP )
            {
#if 0
                mprintf( "scan : %d : %d :%x %x %x %x %x %x\n", advReport->dataLen, 
                    advReport->peerAddrType,
                    advReport->peerBdAddr[0],
                    advReport->peerBdAddr[1],
                    advReport->peerBdAddr[2],
                    advReport->peerBdAddr[3],
                    advReport->peerBdAddr[4],
                    advReport->peerBdAddr[5]
                );
#else
                tary[0] = advReport->peerBdAddr[0];
                tary[1] = advReport->peerBdAddr[1];
                tary[2] = advReport->peerBdAddr[2];
                tary[3] = advReport->peerBdAddr[3];
                tary[4] = advReport->peerBdAddr[4];
                tary[5] = advReport->peerBdAddr[5];
                
                SendPacket( CMD_EVT_PEER, 6, tary );
#endif
            }
            break;
        
        case CYBLE_EVT_GAP_DEVICE_CONNECTED:
            conrsp = (CYBLE_GAP_CONN_PARAM_UPDATED_IN_CONTROLLER_T *)eventParam;
            // mprintf( "connect rsp : %d\n", conrsp->status );
            tary[0] = (uint8)(conrsp->status);
            SendPacket( CMD_CONNECT, 1, tary );
            break;
            
        case CYBLE_EVT_GATTC_WRITE_RSP:
            //mprintf( "notify, enable\n" );
            if ( g_cur_cmd == CMD_ENA_NOTY )
            {
                tary[0] = 0;
                SendPacket( CMD_ENA_NOTY, 1, tary );
            }
            break;
        
        case CYBLE_EVT_GATTC_ERROR_RSP:
            // mprintf( "notify, error rsp\n" );
            if ( g_cur_cmd == CMD_ENA_NOTY )
            {
                tary[0] = 1;
                SendPacket( CMD_ENA_NOTY, 1, tary );
            }
            break;

        case CYBLE_EVT_GATTC_HANDLE_VALUE_NTF:
            notify = (CYBLE_GATTC_HANDLE_VALUE_NTF_PARAM_T *)eventParam;

#if  0
            mprintf( "rrecv NTF : %x,%d, %d : %x %x %x %x %x %x %x %x \n",
                notify->handleValPair.attrHandle,
                notify->handleValPair.value.len,
                notify->handleValPair.value.actualLen,
                notify->handleValPair.value.val[0],
                notify->handleValPair.value.val[1],
                notify->handleValPair.value.val[2],
                notify->handleValPair.value.val[3],
                notify->handleValPair.value.val[4],
                notify->handleValPair.value.val[5],
                notify->handleValPair.value.val[6],
                notify->handleValPair.value.val[7],
                notify->handleValPair.value.val[8]
            );
#else
            tary[0] = notify->handleValPair.attrHandle & 0xff;
            tary[1] = (notify->handleValPair.attrHandle >> 8) & 0xff;
            memcpy( &(tary[2]), notify->handleValPair.value.val, notify->handleValPair.value.len );
            SendPacket( CMD_EVT_NOTY, notify->handleValPair.value.len + 2, tary );
#endif

            break;
            
        case CYBLE_EVT_TIMEOUT:
            preason = (CYBLE_TO_REASON_CODE_T *)eventParam;
            // mprintf( "time out , %d\n", *preason );
            tary[0] = (uint8)(*preason);
            SendPacket( g_cur_cmd, 1, tary );
            break;
        
        /* 在连接状态下, 如果设备下电, 消失了, 会连续发生 2 个下面事件, 66, 40 */
        case CYBLE_EVT_GATT_DISCONNECT_IND:     /* 66 */
            break;
            
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED: /* 40 */
            /* 8 是设备端下电,  22 是中心端主动调用了 CyBle_GapDisconnect */
            // mprintf( "dis connected : %d \n", *(uint8_t *)eventParam );
            if ( g_cur_cmd == CMD_DISCONNT )
            {
                tary[0] = 0;
                SendPacket( CMD_DISCONNT, 1, tary );
            }
            else
            {
                tary[0] = *(uint8_t *)eventParam;
                SendPacket( CMD_EVT_DISC, 1, tary );
            }
            break;
            
        default:
            // mprintf( "other %d\n", event );
            break;
    }
}

/*

0x71 : get state
    resp :

0x55 : start scan.
    resp : 

0x57 : stop scan


0x41: enable notify

0x44: write data

0xcc arg : connect, arg is a mac addr, 6 bytes.
    resp :

0xcd : disconnect.


*/

void  ProcessPacket( uint8 cmd, uint8 tlen, uint8 * pdat )
{
    uint8  tary[64];
    
    /* 无条件执行. */
    if ( (tlen == 0) && (cmd == CMD_GET_STATE) )
    {
        tary[0] = (uint8)cyBle_state;
        SendPacket( CMD_GET_STATE, 1, tary );
        return;
    }
    
    /**/
    if ( cmd == CMD_WRT_NRSP )
    {

        CYBLE_GATTC_WRITE_CMD_REQ_T  uartTxDataWriteCmd;
        uint16  temp;
        
        /**/
        temp = pdat[1];
        temp = ( temp << 8 ) | pdat[0];
        
        /* variable data string */
        uartTxDataWriteCmd.attrHandle = temp;
        uartTxDataWriteCmd.value.len = tlen - 2;
        uartTxDataWriteCmd.value.val = &(pdat[2]);
        
        /**/
        g_cur_cmd = CMD_WRT_NRSP;
        CyBle_GattcWriteWithoutResponse(cyBle_connHandle, &uartTxDataWriteCmd );
        /**/
        tary[0] = 0;
        SendPacket( CMD_WRT_NRSP, 1, tary );
        
        //mprintf( "recv write uart cmd\n" );
        return;
    }
    
    /**/
    if ( tlen == 0 )
    {
        if ( cmd == CMD_STRT_SCAN )
        {
            if ( cyBle_state == CYBLE_STATE_SCANNING )
            {
                tary[0] = 1;
                SendPacket( CMD_STRT_SCAN, 1, tary );
            }
            else
            {
                // mprintf( "recv start scan cmd\n" );
                g_cur_cmd = CMD_STRT_SCAN;
                CyBle_GapcStartScan( CYBLE_SCANNING_FAST );
            }
        }
        else if ( cmd == CMD_STOP_SCAN )
        {
            if ( cyBle_state != CYBLE_STATE_SCANNING )
            {
                tary[0] = 1;
                SendPacket( CMD_STOP_SCAN, 1, tary );
            }
            else
            {
                // mprintf( "recv stop scan cmd\n" );
                g_cur_cmd = CMD_STOP_SCAN;
                CyBle_GapcStopScan();
            }
        }
        else if ( cmd == CMD_DISCONNT )
        {
            if ( cyBle_state != CYBLE_STATE_CONNECTED )
            {
                tary[0] = 1;
                SendPacket( CMD_DISCONNT, 1, tary );
            }
            else
            {
                // mprintf( "recv dis connect cmd\n" );
                g_cur_cmd = CMD_DISCONNT;
                CyBle_GapDisconnect( cyBle_connHandle.bdHandle );
            }
        }
    }
    else if (  tlen == 2 )
    {
        if (  cmd == CMD_ENA_NOTY )
        {
            uint8 enableNotificationParam[2] = { 0x01, 0x00 };
            CYBLE_GATTC_WRITE_REQ_T  enableNotificationReqParam = { {(uint8*)enableNotificationParam, 2, 2}, 0 };
            uint16  temp;
            
            /**/
            temp = pdat[1];
            temp = ( temp << 8 ) | pdat[0];
            
            enableNotificationReqParam.attrHandle = temp;
            g_cur_cmd = CMD_ENA_NOTY;
            CyBle_GattcWriteCharacteristicDescriptors( cyBle_connHandle, (CYBLE_GATTC_WRITE_REQ_T *)(&enableNotificationReqParam) );
            // mprintf( "recv enable notify\n" );
        }
    }
    else if ( tlen == 6 )
    {
        if ( cmd == CMD_CONNECT )
        {
            CYBLE_GAP_BD_ADDR_T  taddr;

            taddr.type = CYBLE_GAP_ADDR_TYPE_PUBLIC;
            taddr.bdAddr[0] = pdat[0];
            taddr.bdAddr[1] = pdat[1];
            taddr.bdAddr[2] = pdat[2];
            taddr.bdAddr[3] = pdat[3];
            taddr.bdAddr[4] = pdat[4];
            taddr.bdAddr[5] = pdat[5];
            
            /**/
            g_cur_cmd = CMD_CONNECT;
            CyBle_GapcConnectDevice( &taddr );

            // mprintf( "recv connect cmd, %d\n", cret  );
        }
    }
    
    return;
}


void  ProcessOneByte( uint8 data )
{
    static uint32  state = 0;
    static uint32  rlen = 0;
    static uint32  rofs = 0;
    static uint8  tcmd;
    static uint8  rbuf[64];

    switch( state )
    {
    case 0:
        if ( data == 0xAA )
        {
            state = 1;
        }
        break;
    
    case 1:
        /* CMD */
        if ( (((data >> 4) ^ data) & 0xF) == 0xF )
        {
            tcmd = data;
            state = 2;
        }
        else
        {
            state = 0;
        }
        break;
    
    
    case 2:
        if ( data == 0 )
        {
            ProcessPacket( tcmd, 0, rbuf );
            state = 0;
        }
        else if ( data < 64 )
        {
            rlen = data;
            rofs = 0;
            state = 3;
        }
        else if ( data == 0xAA )
        {
            state = 1;
        }
        else
        {
            state = 0;
        }
        break;
        
    case 3:
        rbuf[rofs] = data;
        rofs += 1;
        
        if ( rofs >= rlen )
        {
            ProcessPacket( tcmd, rlen, rbuf );
            
            /**/
            state = 0;
        }
        
        break;
    }
    
    return;
}


void  ProcessFromUart( void )
{
    uint32  tlen;
    uint32  i;
    uint8  data;
    
    /**/
    tlen = UART_SpiUartGetRxBufferSize();
    if ( 0 == tlen )
    {
        return;
    }
    
    for ( i=0; i<tlen; i++ )
    {
        data = (uint8) UART_UartGetByte();
        ProcessOneByte( data );
    }
    
    return;
}


int main()
{
    /**/
    CYBLE_API_RESULT_T  bleApiResult;

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_Start();
    
    /* Uncomment this line to enable global interrupts. */
    CyGlobalIntEnable;
    
    /**/
    bleApiResult = CyBle_Start(AppCallBack); 
    if ( bleApiResult != CYBLE_ERROR_OK )
    {
        g_stacks = 1;
        
        for(;;)
        {
            ProcessFromUart();
        }
    }

    /**/
    for(;;)
    {
        /* Place your application code here. */
        CyBle_ProcessEvents();
        ProcessFromUart();
    }
}

/* [] END OF FILE */
