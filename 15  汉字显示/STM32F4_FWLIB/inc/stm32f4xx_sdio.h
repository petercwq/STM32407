/**
  ******************************************************************************
  * @file    stm32f4xx_sdio.h
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   This file contains all the functions prototypes for the SDIO firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_SDIO_H
#define __STM32F4xx_SDIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup SDIO
  * @{
  */

/* Exported types ------------------------------------------------------------*/

typedef struct
{
  uint32_t SDIO_ClockEdge;            /*!< Specifies the clock transition on which the bit capture is made.
                                           This parameter can be a value of @ref SDIO_Clock_Edge */

  uint32_t SDIO_ClockBypass;          /*!< Specifies whether the SDIO Clock divider bypass is
                                           enabled or disabled.
                                           This parameter can be a value of @ref SDIO_Clock_Bypass */

  uint32_t SDIO_ClockPowerSave;       /*!< Specifies whether SDIO Clock output is enabled or
                                           disabled when the bus is idle.
                                           This parameter can be a value of @ref SDIO_Clock_Power_Save */

  uint32_t SDIO_BusWide;              /*!< Specifies the SDIO bus width.
                                           This parameter can be a value of @ref SDIO_Bus_Wide */

  uint32_t SDIO_HardwareFlowControl;  /*!< Specifies whether the SDIO hardware flow control is enabled or disabled.
                                           This parameter can be a value of @ref SDIO_Hardware_Flow_Control */

  uint8_t SDIO_ClockDiv;              /*!< Specifies the clock frequency of the SDIO controller.
                                           This parameter can be a value between 0x00 and 0xFF. */
                                           
} SDIO_InitTypeDef;

typedef struct
{
  uint32_t SDIO_Argument;  /*!< Specifies the SDIO command argument which is sent
                                to a card as part of a command message. If a command
                                contains an argument, it must be loaded into this register
                                before writing the command to the command register */

  uint32_t SDIO_CmdIndex;  /*!< Specifies the SDIO command index. It must be lower than 0x40. */

  uint32_t SDIO_Response;  /*!< Specifies the SDIO response type.
                                This parameter can be a value of @ref SDIO_Response_Type */

  uint32_t SDIO_Wait;      /*!< Specifies whether SDIO wait for interrupt request is enabled or disabled.
                                This parameter can be a value of @ref SDIO_Wait_Interrupt_State */

  uint32_t SDIO_CPSM;      /*!< Specifies whether SDIO Command path state machine (CPSM)
                                is enabled or disabled.
                                This parameter can be a value of @ref SDIO_CPSM_State */
} SDIO_CmdInitTypeDef;

typedef struct
{
  uint32_t SDIO_DataTimeOut;    /*!< Specifies the data timeout period in card bus clock periods. */

  uint32_t SDIO_DataLength;     /*!< Specifies the number of data bytes to be transferred. */
 
  uint32_t SDIO_DataBlockSize;  /*!< Specifies the data block size for block transfer.
                                     This parameter can be a value of @ref SDIO_Data_Block_Size */
 
  uint32_t SDIO_TransferDir;    /*!< Specifies the data transfer direction, whether the transfer
                                     is a read or write.
                                     This parameter can be a value of @ref SDIO_Transfer_Direction */
 
  uint32_t SDIO_TransferMode;   /*!< Specifies whether data transfer is in stream or block mode.
                                     This parameter can be a value of @ref SDIO_Transfer_Type */
 
  uint32_t SDIO_DPSM;           /*!< Specifies whether SDIO Data path state machine (DPSM)
                                     is enabled or disabled.
                                     This parameter can be a value of @ref SDIO_DPSM_State */
} SDIO_DataInitTypeDef;

//SD卡CSD寄存器数据		  
typedef struct
{
	uint8_t  CSDStruct;            /*!< CSD structure */
	uint8_t  SysSpecVersion;       /*!< System specification version */
	uint8_t  Reserved1;            /*!< Reserved */
	uint8_t  TAAC;                 /*!< Data read access-time 1 */
	uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
	uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
	uint16_t CardComdClasses;      /*!< Card command classes */
	uint8_t  RdBlockLen;           /*!< Max. read data block length */
	uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
	uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
	uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
	uint8_t  DSRImpl;              /*!< DSR implemented */
	uint8_t  Reserved2;            /*!< Reserved */
	uint32_t DeviceSize;           /*!< Device Size */
	uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
	uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
	uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
	uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
	uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
	uint8_t  EraseGrSize;          /*!< Erase group size */
	uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
	uint8_t  WrProtectGrSize;      /*!< Write protect group size */
	uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
	uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
	uint8_t  WrSpeedFact;          /*!< Write speed factor */
	uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
	uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
	uint8_t  Reserved3;            /*!< Reserded */
	uint8_t  ContentProtectAppli;  /*!< Content protection application */
	uint8_t  FileFormatGrouop;     /*!< File format group */
	uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
	uint8_t  PermWrProtect;        /*!< Permanent write protection */
	uint8_t  TempWrProtect;        /*!< Temporary write protection */
	uint8_t  FileFormat;           /*!< File Format */
	uint8_t  ECC;                  /*!< ECC code */
	uint8_t  CSD_CRC;              /*!< CSD CRC */
	uint8_t  Reserved4;            /*!< always 1*/
} SD_CSD;   

//SD卡CID寄存器数据
typedef struct
{
	uint8_t  ManufacturerID;       /*!< ManufacturerID */
	uint16_t OEM_AppliID;          /*!< OEM/Application ID */
	uint32_t ProdName1;            /*!< Product Name part1 */
	uint8_t  ProdName2;            /*!< Product Name part2*/
	uint8_t  ProdRev;              /*!< Product Revision */
	uint32_t ProdSN;               /*!< Product Serial Number */
	uint8_t  Reserved1;            /*!< Reserved1 */
	uint16_t ManufactDate;         /*!< Manufacturing Date */
	uint8_t  CID_CRC;              /*!< CID CRC */
	uint8_t  Reserved2;            /*!< always 1 */
} SD_CID;	 

//SDIO 各种错误枚举定义
typedef enum
{	 
	//特殊错误定义 
	SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
	SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
	SD_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
	SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
	SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
	SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
	SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
	SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
	SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
	SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
	SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
	SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
	SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
	SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
	SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
	SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
	SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
	SD_CC_ERROR                        = (18), /*!< Internal card controller error */
	SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
	SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
	SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
	SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
	SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
	SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
	SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
	SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
	SD_INVALID_VOLTRANGE               = (27),
	SD_ADDR_OUT_OF_RANGE               = (28),
	SD_SWITCH_ERROR                    = (29),
	SD_SDIO_DISABLED                   = (30),
	SD_SDIO_FUNCTION_BUSY              = (31),
	SD_SDIO_FUNCTION_FAILED            = (32),
	SD_SDIO_UNKNOWN_FUNCTION           = (33),
	//标准错误定义
	SD_INTERNAL_ERROR, 
	SD_NOT_CONFIGURED,
	SD_REQUEST_PENDING, 
	SD_REQUEST_NOT_APPLICABLE, 
	SD_INVALID_PARAMETER,  
	SD_UNSUPPORTED_FEATURE,  
	SD_UNSUPPORTED_HW,  
	SD_ERROR,  
	SD_OK = 0 
} SD_Error;

//SD卡状态
typedef enum
{
	SD_CARD_READY                  = ((uint32_t)0x00000001),
	SD_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
	SD_CARD_STANDBY                = ((uint32_t)0x00000003),
	SD_CARD_TRANSFER               = ((uint32_t)0x00000004),
	SD_CARD_SENDING                = ((uint32_t)0x00000005),
	SD_CARD_RECEIVING              = ((uint32_t)0x00000006),
	SD_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
	SD_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
	SD_CARD_ERROR                  = ((uint32_t)0x000000FF)
}SDCardState;

////////////////////////////////////////////////////////////////////////////////////////////////////
//SDIO 指令集
#define SD_CMD_GO_IDLE_STATE                       ((u8)0)
#define SD_CMD_SEND_OP_COND                        ((u8)1)
#define SD_CMD_ALL_SEND_CID                        ((u8)2)
#define SD_CMD_SET_REL_ADDR                        ((u8)3) /*!< SDIO_SEND_REL_ADDR for SD Card */
#define SD_CMD_SET_DSR                             ((u8)4)
#define SD_CMD_SDIO_SEN_OP_COND                    ((u8)5)
#define SD_CMD_HS_SWITCH                           ((u8)6)
#define SD_CMD_SEL_DESEL_CARD                      ((u8)7)
#define SD_CMD_HS_SEND_EXT_CSD                     ((u8)8)
#define SD_CMD_SEND_CSD                            ((u8)9)
#define SD_CMD_SEND_CID                            ((u8)10)
#define SD_CMD_READ_DAT_UNTIL_STOP                 ((u8)11) /*!< SD Card doesn't support it */
#define SD_CMD_STOP_TRANSMISSION                   ((u8)12)
#define SD_CMD_SEND_STATUS                         ((u8)13)
#define SD_CMD_HS_BUSTEST_READ                     ((u8)14)
#define SD_CMD_GO_INACTIVE_STATE                   ((u8)15)
#define SD_CMD_SET_BLOCKLEN                        ((u8)16)
#define SD_CMD_READ_SINGLE_BLOCK                   ((u8)17)
#define SD_CMD_READ_MULT_BLOCK                     ((u8)18)
#define SD_CMD_HS_BUSTEST_WRITE                    ((u8)19)
#define SD_CMD_WRITE_DAT_UNTIL_STOP                ((u8)20) 
#define SD_CMD_SET_BLOCK_COUNT                     ((u8)23) 
#define SD_CMD_WRITE_SINGLE_BLOCK                  ((u8)24)
#define SD_CMD_WRITE_MULT_BLOCK                    ((u8)25)
#define SD_CMD_PROG_CID                            ((u8)26)
#define SD_CMD_PROG_CSD                            ((u8)27)
#define SD_CMD_SET_WRITE_PROT                      ((u8)28)
#define SD_CMD_CLR_WRITE_PROT                      ((u8)29)
#define SD_CMD_SEND_WRITE_PROT                     ((u8)30)
#define SD_CMD_SD_ERASE_GRP_START                  ((u8)32) /*!< To set the address of the first write
                                                                  block to be erased. (For SD card only) */
#define SD_CMD_SD_ERASE_GRP_END                    ((u8)33) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For SD card only) */
#define SD_CMD_ERASE_GRP_START                     ((u8)35) /*!< To set the address of the first write block to be erased.
                                                                  (For MMC card only spec 3.31) */

#define SD_CMD_ERASE_GRP_END                       ((u8)36) /*!< To set the address of the last write block of the
                                                                  continuous range to be erased. (For MMC card only spec 3.31) */

#define SD_CMD_ERASE                               ((u8)38)
#define SD_CMD_FAST_IO                             ((u8)39) /*!< SD Card doesn't support it */
#define SD_CMD_GO_IRQ_STATE                        ((u8)40) /*!< SD Card doesn't support it */
#define SD_CMD_LOCK_UNLOCK                         ((u8)42)
#define SD_CMD_APP_CMD                             ((u8)55)
#define SD_CMD_GEN_CMD                             ((u8)56)
#define SD_CMD_NO_CMD                              ((u8)64)

/** 
  * @brief Following commands are SD Card Specific commands.
  *        SDIO_APP_CMD ：CMD55 should be sent before sending these commands. 
  */
#define SD_CMD_APP_SD_SET_BUSWIDTH                 ((u8)6)  /*!< For SD Card only */
#define SD_CMD_SD_APP_STAUS                        ((u8)13) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS        ((u8)22) /*!< For SD Card only */
#define SD_CMD_SD_APP_OP_COND                      ((u8)41) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CLR_CARD_DETECT          ((u8)42) /*!< For SD Card only */
#define SD_CMD_SD_APP_SEND_SCR                     ((u8)51) /*!< For SD Card only */
#define SD_CMD_SDIO_RW_DIRECT                      ((u8)52) /*!< For SD I/O Card only */
#define SD_CMD_SDIO_RW_EXTENDED                    ((u8)53) /*!< For SD I/O Card only */

/** 
  * @brief Following commands are SD Card Specific security commands.
  *        SDIO_APP_CMD should be sent before sending these commands. 
  */
#define SD_CMD_SD_APP_GET_MKB                      ((u8)43) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_MID                      ((u8)44) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RN1                  ((u8)45) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RN2                  ((u8)46) /*!< For SD Card only */
#define SD_CMD_SD_APP_SET_CER_RES2                 ((u8)47) /*!< For SD Card only */
#define SD_CMD_SD_APP_GET_CER_RES1                 ((u8)48) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK   ((u8)18) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK  ((u8)25) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_ERASE                 ((u8)38) /*!< For SD Card only */
#define SD_CMD_SD_APP_CHANGE_SECURE_AREA           ((u8)49) /*!< For SD Card only */
#define SD_CMD_SD_APP_SECURE_WRITE_MKB             ((u8)48) /*!< For SD Card only */
  			   
//支持的SD卡定义
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((u32)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((u32)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((u32)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((u32)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((u32)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((u32)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((u32)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((u32)0x00000007)

//SDIO相关参数定义
#define NULL 0
#define SDIO_STATIC_FLAGS               ((u32)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((u32)0x00010000)	  
#define SDIO_DATATIMEOUT                ((u32)0xFFFFFFFF)	  
#define SDIO_FIFO_Address               ((u32)0x40018080)

//Mask for errors Card Status R1 (OCR Register)  
#define SD_OCR_ADDR_OUT_OF_RANGE        ((u32)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((u32)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((u32)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((u32)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((u32)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((u32)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((u32)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((u32)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((u32)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((u32)0x00200000)
#define SD_OCR_CC_ERROR                 ((u32)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((u32)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((u32)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((u32)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((u32)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((u32)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((u32)0x00004000)
#define SD_OCR_ERASE_RESET              ((u32)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((u32)0x00000008)
#define SD_OCR_ERRORBITS                ((u32)0xFDFFE008)

//Masks for R6 Response 
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((u32)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((u32)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((u32)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((u32)0x80100000)
#define SD_HIGH_CAPACITY                ((u32)0x40000000)
#define SD_STD_CAPACITY                 ((u32)0x00000000)
#define SD_CHECK_PATTERN                ((u32)0x000001AA)
#define SD_VOLTAGE_WINDOW_MMC           ((u32)0x80FF8000)

#define SD_MAX_VOLT_TRIAL               ((u32)0x0000FFFF)
#define SD_ALLZERO                      ((u32)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((u32)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((u32)0x00010000)
#define SD_CARD_LOCKED                  ((u32)0x02000000)
#define SD_CARD_PROGRAMMING             ((u32)0x00000007)
#define SD_CARD_RECEIVING               ((u32)0x00000006)
#define SD_DATATIMEOUT                  ((u32)0xFFFFFFFF)
#define SD_0TO7BITS                     ((u32)0x000000FF)
#define SD_8TO15BITS                    ((u32)0x0000FF00)
#define SD_16TO23BITS                   ((u32)0x00FF0000)
#define SD_24TO31BITS                   ((u32)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((u32)0x01FFFFFF)

#define SD_HALFFIFO                     ((u32)0x00000008)
#define SD_HALFFIFOBYTES                ((u32)0x00000020)

//Command Class Supported  
#define SD_CCCC_LOCK_UNLOCK             ((u32)0x00000080)
#define SD_CCCC_WRITE_PROT              ((u32)0x00000040)
#define SD_CCCC_ERASE                   ((u32)0x00000020)
																	 
//CMD8指令
#define SDIO_SEND_IF_COND               ((u32)0x00000008)
//////////////////////////////////////////////////////////////////////////////////////////////////////
/* Exported constants --------------------------------------------------------*/


/** @defgroup SDIO_Exported_Constants
  * @{
  */

/** @defgroup SDIO_Clock_Edge 
  * @{
  */

#define SDIO_ClockEdge_Rising               ((uint32_t)0x00000000)
#define SDIO_ClockEdge_Falling              ((uint32_t)0x00002000)
#define IS_SDIO_CLOCK_EDGE(EDGE) (((EDGE) == SDIO_ClockEdge_Rising) || \
                                  ((EDGE) == SDIO_ClockEdge_Falling))
/**
  * @}
  */

/** @defgroup SDIO_Clock_Bypass 
  * @{
  */

#define SDIO_ClockBypass_Disable             ((uint32_t)0x00000000)
#define SDIO_ClockBypass_Enable              ((uint32_t)0x00000400)    
#define IS_SDIO_CLOCK_BYPASS(BYPASS) (((BYPASS) == SDIO_ClockBypass_Disable) || \
                                     ((BYPASS) == SDIO_ClockBypass_Enable))
/**
  * @}
  */ 

/** @defgroup SDIO_Clock_Power_Save 
  * @{
  */

#define SDIO_ClockPowerSave_Disable         ((uint32_t)0x00000000)
#define SDIO_ClockPowerSave_Enable          ((uint32_t)0x00000200) 
#define IS_SDIO_CLOCK_POWER_SAVE(SAVE) (((SAVE) == SDIO_ClockPowerSave_Disable) || \
                                        ((SAVE) == SDIO_ClockPowerSave_Enable))
/**
  * @}
  */

/** @defgroup SDIO_Bus_Wide 
  * @{
  */

#define SDIO_BusWide_1b                     ((uint32_t)0x00000000)
#define SDIO_BusWide_4b                     ((uint32_t)0x00000800)
#define SDIO_BusWide_8b                     ((uint32_t)0x00001000)
#define IS_SDIO_BUS_WIDE(WIDE) (((WIDE) == SDIO_BusWide_1b) || ((WIDE) == SDIO_BusWide_4b) || \
                                ((WIDE) == SDIO_BusWide_8b))

/**
  * @}
  */

/** @defgroup SDIO_Hardware_Flow_Control 
  * @{
  */

#define SDIO_HardwareFlowControl_Disable    ((uint32_t)0x00000000)
#define SDIO_HardwareFlowControl_Enable     ((uint32_t)0x00004000)
#define IS_SDIO_HARDWARE_FLOW_CONTROL(CONTROL) (((CONTROL) == SDIO_HardwareFlowControl_Disable) || \
                                                ((CONTROL) == SDIO_HardwareFlowControl_Enable))
/**
  * @}
  */

/** @defgroup SDIO_Power_State 
  * @{
  */

#define SDIO_PowerState_OFF                 ((uint32_t)0x00000000)
#define SDIO_PowerState_ON                  ((uint32_t)0x00000003)
#define IS_SDIO_POWER_STATE(STATE) (((STATE) == SDIO_PowerState_OFF) || ((STATE) == SDIO_PowerState_ON))
/**
  * @}
  */ 


/** @defgroup SDIO_Interrupt_sources
  * @{
  */

#define SDIO_IT_CCRCFAIL                    ((uint32_t)0x00000001)
#define SDIO_IT_DCRCFAIL                    ((uint32_t)0x00000002)
#define SDIO_IT_CTIMEOUT                    ((uint32_t)0x00000004)
#define SDIO_IT_DTIMEOUT                    ((uint32_t)0x00000008)
#define SDIO_IT_TXUNDERR                    ((uint32_t)0x00000010)
#define SDIO_IT_RXOVERR                     ((uint32_t)0x00000020)
#define SDIO_IT_CMDREND                     ((uint32_t)0x00000040)
#define SDIO_IT_CMDSENT                     ((uint32_t)0x00000080)
#define SDIO_IT_DATAEND                     ((uint32_t)0x00000100)
#define SDIO_IT_STBITERR                    ((uint32_t)0x00000200)
#define SDIO_IT_DBCKEND                     ((uint32_t)0x00000400)
#define SDIO_IT_CMDACT                      ((uint32_t)0x00000800)
#define SDIO_IT_TXACT                       ((uint32_t)0x00001000)
#define SDIO_IT_RXACT                       ((uint32_t)0x00002000)
#define SDIO_IT_TXFIFOHE                    ((uint32_t)0x00004000)
#define SDIO_IT_RXFIFOHF                    ((uint32_t)0x00008000)
#define SDIO_IT_TXFIFOF                     ((uint32_t)0x00010000)
#define SDIO_IT_RXFIFOF                     ((uint32_t)0x00020000)
#define SDIO_IT_TXFIFOE                     ((uint32_t)0x00040000)
#define SDIO_IT_RXFIFOE                     ((uint32_t)0x00080000)
#define SDIO_IT_TXDAVL                      ((uint32_t)0x00100000)
#define SDIO_IT_RXDAVL                      ((uint32_t)0x00200000)
#define SDIO_IT_SDIOIT                      ((uint32_t)0x00400000)
#define SDIO_IT_CEATAEND                    ((uint32_t)0x00800000)
#define IS_SDIO_IT(IT) ((((IT) & (uint32_t)0xFF000000) == 0x00) && ((IT) != (uint32_t)0x00))
/**
  * @}
  */ 

/** @defgroup SDIO_Command_Index
  * @{
  */

#define IS_SDIO_CMD_INDEX(INDEX)            ((INDEX) < 0x40)
/**
  * @}
  */

/** @defgroup SDIO_Response_Type
  * @{
  */

#define SDIO_Response_No                    ((uint32_t)0x00000000)
#define SDIO_Response_Short                 ((uint32_t)0x00000040)
#define SDIO_Response_Long                  ((uint32_t)0x000000C0)
#define IS_SDIO_RESPONSE(RESPONSE) (((RESPONSE) == SDIO_Response_No) || \
                                    ((RESPONSE) == SDIO_Response_Short) || \
                                    ((RESPONSE) == SDIO_Response_Long))
/**
  * @}
  */

/** @defgroup SDIO_Wait_Interrupt_State
  * @{
  */

#define SDIO_Wait_No                        ((uint32_t)0x00000000) /*!< SDIO No Wait, TimeOut is enabled */
#define SDIO_Wait_IT                        ((uint32_t)0x00000100) /*!< SDIO Wait Interrupt Request */
#define SDIO_Wait_Pend                      ((uint32_t)0x00000200) /*!< SDIO Wait End of transfer */
#define IS_SDIO_WAIT(WAIT) (((WAIT) == SDIO_Wait_No) || ((WAIT) == SDIO_Wait_IT) || \
                            ((WAIT) == SDIO_Wait_Pend))
/**
  * @}
  */

/** @defgroup SDIO_CPSM_State
  * @{
  */

#define SDIO_CPSM_Disable                    ((uint32_t)0x00000000)
#define SDIO_CPSM_Enable                     ((uint32_t)0x00000400)
#define IS_SDIO_CPSM(CPSM) (((CPSM) == SDIO_CPSM_Enable) || ((CPSM) == SDIO_CPSM_Disable))
/**
  * @}
  */ 

/** @defgroup SDIO_Response_Registers
  * @{
  */

#define SDIO_RESP1                          ((uint32_t)0x00000000)
#define SDIO_RESP2                          ((uint32_t)0x00000004)
#define SDIO_RESP3                          ((uint32_t)0x00000008)
#define SDIO_RESP4                          ((uint32_t)0x0000000C)
#define IS_SDIO_RESP(RESP) (((RESP) == SDIO_RESP1) || ((RESP) == SDIO_RESP2) || \
                            ((RESP) == SDIO_RESP3) || ((RESP) == SDIO_RESP4))
/**
  * @}
  */

/** @defgroup SDIO_Data_Length 
  * @{
  */

#define IS_SDIO_DATA_LENGTH(LENGTH) ((LENGTH) <= 0x01FFFFFF)
/**
  * @}
  */

/** @defgroup SDIO_Data_Block_Size 
  * @{
  */

#define SDIO_DataBlockSize_1b               ((uint32_t)0x00000000)
#define SDIO_DataBlockSize_2b               ((uint32_t)0x00000010)
#define SDIO_DataBlockSize_4b               ((uint32_t)0x00000020)
#define SDIO_DataBlockSize_8b               ((uint32_t)0x00000030)
#define SDIO_DataBlockSize_16b              ((uint32_t)0x00000040)
#define SDIO_DataBlockSize_32b              ((uint32_t)0x00000050)
#define SDIO_DataBlockSize_64b              ((uint32_t)0x00000060)
#define SDIO_DataBlockSize_128b             ((uint32_t)0x00000070)
#define SDIO_DataBlockSize_256b             ((uint32_t)0x00000080)
#define SDIO_DataBlockSize_512b             ((uint32_t)0x00000090)
#define SDIO_DataBlockSize_1024b            ((uint32_t)0x000000A0)
#define SDIO_DataBlockSize_2048b            ((uint32_t)0x000000B0)
#define SDIO_DataBlockSize_4096b            ((uint32_t)0x000000C0)
#define SDIO_DataBlockSize_8192b            ((uint32_t)0x000000D0)
#define SDIO_DataBlockSize_16384b           ((uint32_t)0x000000E0)
#define IS_SDIO_BLOCK_SIZE(SIZE) (((SIZE) == SDIO_DataBlockSize_1b) || \
                                  ((SIZE) == SDIO_DataBlockSize_2b) || \
                                  ((SIZE) == SDIO_DataBlockSize_4b) || \
                                  ((SIZE) == SDIO_DataBlockSize_8b) || \
                                  ((SIZE) == SDIO_DataBlockSize_16b) || \
                                  ((SIZE) == SDIO_DataBlockSize_32b) || \
                                  ((SIZE) == SDIO_DataBlockSize_64b) || \
                                  ((SIZE) == SDIO_DataBlockSize_128b) || \
                                  ((SIZE) == SDIO_DataBlockSize_256b) || \
                                  ((SIZE) == SDIO_DataBlockSize_512b) || \
                                  ((SIZE) == SDIO_DataBlockSize_1024b) || \
                                  ((SIZE) == SDIO_DataBlockSize_2048b) || \
                                  ((SIZE) == SDIO_DataBlockSize_4096b) || \
                                  ((SIZE) == SDIO_DataBlockSize_8192b) || \
                                  ((SIZE) == SDIO_DataBlockSize_16384b)) 
/**
  * @}
  */

/** @defgroup SDIO_Transfer_Direction 
  * @{
  */

#define SDIO_TransferDir_ToCard             ((uint32_t)0x00000000)
#define SDIO_TransferDir_ToSDIO             ((uint32_t)0x00000002)
#define IS_SDIO_TRANSFER_DIR(DIR) (((DIR) == SDIO_TransferDir_ToCard) || \
                                   ((DIR) == SDIO_TransferDir_ToSDIO))
/**
  * @}
  */

/** @defgroup SDIO_Transfer_Type 
  * @{
  */

#define SDIO_TransferMode_Block             ((uint32_t)0x00000000)
#define SDIO_TransferMode_Stream            ((uint32_t)0x00000004)
#define IS_SDIO_TRANSFER_MODE(MODE) (((MODE) == SDIO_TransferMode_Stream) || \
                                     ((MODE) == SDIO_TransferMode_Block))
/**
  * @}
  */

/** @defgroup SDIO_DPSM_State 
  * @{
  */

#define SDIO_DPSM_Disable                    ((uint32_t)0x00000000)
#define SDIO_DPSM_Enable                     ((uint32_t)0x00000001)
#define IS_SDIO_DPSM(DPSM) (((DPSM) == SDIO_DPSM_Enable) || ((DPSM) == SDIO_DPSM_Disable))
/**
  * @}
  */

/** @defgroup SDIO_Flags 
  * @{
  */

#define SDIO_FLAG_CCRCFAIL                  ((uint32_t)0x00000001)
#define SDIO_FLAG_DCRCFAIL                  ((uint32_t)0x00000002)
#define SDIO_FLAG_CTIMEOUT                  ((uint32_t)0x00000004)
#define SDIO_FLAG_DTIMEOUT                  ((uint32_t)0x00000008)
#define SDIO_FLAG_TXUNDERR                  ((uint32_t)0x00000010)
#define SDIO_FLAG_RXOVERR                   ((uint32_t)0x00000020)
#define SDIO_FLAG_CMDREND                   ((uint32_t)0x00000040)
#define SDIO_FLAG_CMDSENT                   ((uint32_t)0x00000080)
#define SDIO_FLAG_DATAEND                   ((uint32_t)0x00000100)
#define SDIO_FLAG_STBITERR                  ((uint32_t)0x00000200)
#define SDIO_FLAG_DBCKEND                   ((uint32_t)0x00000400)
#define SDIO_FLAG_CMDACT                    ((uint32_t)0x00000800)
#define SDIO_FLAG_TXACT                     ((uint32_t)0x00001000)
#define SDIO_FLAG_RXACT                     ((uint32_t)0x00002000)
#define SDIO_FLAG_TXFIFOHE                  ((uint32_t)0x00004000)
#define SDIO_FLAG_RXFIFOHF                  ((uint32_t)0x00008000)
#define SDIO_FLAG_TXFIFOF                   ((uint32_t)0x00010000)
#define SDIO_FLAG_RXFIFOF                   ((uint32_t)0x00020000)
#define SDIO_FLAG_TXFIFOE                   ((uint32_t)0x00040000)
#define SDIO_FLAG_RXFIFOE                   ((uint32_t)0x00080000)
#define SDIO_FLAG_TXDAVL                    ((uint32_t)0x00100000)
#define SDIO_FLAG_RXDAVL                    ((uint32_t)0x00200000)
#define SDIO_FLAG_SDIOIT                    ((uint32_t)0x00400000)
#define SDIO_FLAG_CEATAEND                  ((uint32_t)0x00800000)
#define IS_SDIO_FLAG(FLAG) (((FLAG)  == SDIO_FLAG_CCRCFAIL) || \
                            ((FLAG)  == SDIO_FLAG_DCRCFAIL) || \
                            ((FLAG)  == SDIO_FLAG_CTIMEOUT) || \
                            ((FLAG)  == SDIO_FLAG_DTIMEOUT) || \
                            ((FLAG)  == SDIO_FLAG_TXUNDERR) || \
                            ((FLAG)  == SDIO_FLAG_RXOVERR) || \
                            ((FLAG)  == SDIO_FLAG_CMDREND) || \
                            ((FLAG)  == SDIO_FLAG_CMDSENT) || \
                            ((FLAG)  == SDIO_FLAG_DATAEND) || \
                            ((FLAG)  == SDIO_FLAG_STBITERR) || \
                            ((FLAG)  == SDIO_FLAG_DBCKEND) || \
                            ((FLAG)  == SDIO_FLAG_CMDACT) || \
                            ((FLAG)  == SDIO_FLAG_TXACT) || \
                            ((FLAG)  == SDIO_FLAG_RXACT) || \
                            ((FLAG)  == SDIO_FLAG_TXFIFOHE) || \
                            ((FLAG)  == SDIO_FLAG_RXFIFOHF) || \
                            ((FLAG)  == SDIO_FLAG_TXFIFOF) || \
                            ((FLAG)  == SDIO_FLAG_RXFIFOF) || \
                            ((FLAG)  == SDIO_FLAG_TXFIFOE) || \
                            ((FLAG)  == SDIO_FLAG_RXFIFOE) || \
                            ((FLAG)  == SDIO_FLAG_TXDAVL) || \
                            ((FLAG)  == SDIO_FLAG_RXDAVL) || \
                            ((FLAG)  == SDIO_FLAG_SDIOIT) || \
                            ((FLAG)  == SDIO_FLAG_CEATAEND))

#define IS_SDIO_CLEAR_FLAG(FLAG) ((((FLAG) & (uint32_t)0xFF3FF800) == 0x00) && ((FLAG) != (uint32_t)0x00))

#define IS_SDIO_GET_IT(IT) (((IT)  == SDIO_IT_CCRCFAIL) || \
                            ((IT)  == SDIO_IT_DCRCFAIL) || \
                            ((IT)  == SDIO_IT_CTIMEOUT) || \
                            ((IT)  == SDIO_IT_DTIMEOUT) || \
                            ((IT)  == SDIO_IT_TXUNDERR) || \
                            ((IT)  == SDIO_IT_RXOVERR) || \
                            ((IT)  == SDIO_IT_CMDREND) || \
                            ((IT)  == SDIO_IT_CMDSENT) || \
                            ((IT)  == SDIO_IT_DATAEND) || \
                            ((IT)  == SDIO_IT_STBITERR) || \
                            ((IT)  == SDIO_IT_DBCKEND) || \
                            ((IT)  == SDIO_IT_CMDACT) || \
                            ((IT)  == SDIO_IT_TXACT) || \
                            ((IT)  == SDIO_IT_RXACT) || \
                            ((IT)  == SDIO_IT_TXFIFOHE) || \
                            ((IT)  == SDIO_IT_RXFIFOHF) || \
                            ((IT)  == SDIO_IT_TXFIFOF) || \
                            ((IT)  == SDIO_IT_RXFIFOF) || \
                            ((IT)  == SDIO_IT_TXFIFOE) || \
                            ((IT)  == SDIO_IT_RXFIFOE) || \
                            ((IT)  == SDIO_IT_TXDAVL) || \
                            ((IT)  == SDIO_IT_RXDAVL) || \
                            ((IT)  == SDIO_IT_SDIOIT) || \
                            ((IT)  == SDIO_IT_CEATAEND))

#define IS_SDIO_CLEAR_IT(IT) ((((IT) & (uint32_t)0xFF3FF800) == 0x00) && ((IT) != (uint32_t)0x00))

/**
  * @}
  */

/** @defgroup SDIO_Read_Wait_Mode 
  * @{
  */

#define SDIO_ReadWaitMode_DATA2             ((uint32_t)0x00000000)
#define SDIO_ReadWaitMode_CLK               ((uint32_t)0x00000001)
#define IS_SDIO_READWAIT_MODE(MODE) (((MODE) == SDIO_ReadWaitMode_CLK) || \
                                     ((MODE) == SDIO_ReadWaitMode_DATA2))
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/*  Function used to set the SDIO configuration to the default reset state ****/
void SDIO_DeInit(void);

/* Initialization and Configuration functions *********************************/
void SDIO_Init(SDIO_InitTypeDef* SDIO_InitStruct);
void SDIO_StructInit(SDIO_InitTypeDef* SDIO_InitStruct);
void SDIO_ClockCmd(FunctionalState NewState);
void SDIO_SetPowerState(uint32_t SDIO_PowerState);
uint32_t SDIO_GetPowerState(void);

/* Command path state machine (CPSM) management functions *********************/
void SDIO_SendCommand(SDIO_CmdInitTypeDef *SDIO_CmdInitStruct);
void SDIO_CmdStructInit(SDIO_CmdInitTypeDef* SDIO_CmdInitStruct);
uint8_t SDIO_GetCommandResponse(void);
uint32_t SDIO_GetResponse(uint32_t SDIO_RESP);

/* Data path state machine (DPSM) management functions ************************/
void SDIO_DataConfig(SDIO_DataInitTypeDef* SDIO_DataInitStruct);
void SDIO_DataStructInit(SDIO_DataInitTypeDef* SDIO_DataInitStruct);
uint32_t SDIO_GetDataCounter(void);
uint32_t SDIO_ReadData(void);
void SDIO_WriteData(uint32_t Data);
uint32_t SDIO_GetFIFOCount(void);

/* SDIO IO Cards mode management functions ************************************/
void SDIO_StartSDIOReadWait(FunctionalState NewState);
void SDIO_StopSDIOReadWait(FunctionalState NewState);
void SDIO_SetSDIOReadWaitMode(uint32_t SDIO_ReadWaitMode);
void SDIO_SetSDIOOperation(FunctionalState NewState);
void SDIO_SendSDIOSuspendCmd(FunctionalState NewState);

/* CE-ATA mode management functions *******************************************/
void SDIO_CommandCompletionCmd(FunctionalState NewState);
void SDIO_CEATAITCmd(FunctionalState NewState);
void SDIO_SendCEATACmd(FunctionalState NewState);

/* DMA transfers management functions *****************************************/
void SDIO_DMACmd(FunctionalState NewState);

/* Interrupts and flags management functions **********************************/
void SDIO_ITConfig(uint32_t SDIO_IT, FunctionalState NewState);
FlagStatus SDIO_GetFlagStatus(uint32_t SDIO_FLAG);
void SDIO_ClearFlag(uint32_t SDIO_FLAG);
ITStatus SDIO_GetITStatus(uint32_t SDIO_IT);
void SDIO_ClearITPendingBit(uint32_t SDIO_IT);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_SDIO_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
