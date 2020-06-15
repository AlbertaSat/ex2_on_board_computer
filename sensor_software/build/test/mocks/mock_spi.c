/* AUTOGENERATED FILE. DO NOT EDIT. */
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "cmock.h"
#include "mock_spi.h"

static const char* CMockString_blocksize = "blocksize";
static const char* CMockString_destbuff = "destbuff";
static const char* CMockString_spi_read = "spi_read";
static const char* CMockString_spi_write = "spi_write";
static const char* CMockString_srcbuff = "srcbuff";

typedef struct _CMOCK_spi_write_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ExpectAnyArgsBool;
  int CallOrder;
  uint32_t Expected_blocksize;
  uint16_t* Expected_srcbuff;

} CMOCK_spi_write_CALL_INSTANCE;

typedef struct _CMOCK_spi_read_CALL_INSTANCE
{
  UNITY_LINE_TYPE LineNumber;
  char ExpectAnyArgsBool;
  int CallOrder;
  uint32_t Expected_blocksize;
  uint16_t* Expected_destbuff;

} CMOCK_spi_read_CALL_INSTANCE;

static struct mock_spiInstance
{
  char spi_write_IgnoreBool;
  char spi_write_CallbackBool;
  CMOCK_spi_write_CALLBACK spi_write_CallbackFunctionPointer;
  int spi_write_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE spi_write_CallInstance;
  char spi_read_IgnoreBool;
  char spi_read_CallbackBool;
  CMOCK_spi_read_CALLBACK spi_read_CallbackFunctionPointer;
  int spi_read_CallbackCalls;
  CMOCK_MEM_INDEX_TYPE spi_read_CallInstance;
} Mock;

extern jmp_buf AbortFrame;
extern int GlobalExpectCount;
extern int GlobalVerifyOrder;

void mock_spi_Verify(void)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_MEM_INDEX_TYPE call_instance;
  call_instance = Mock.spi_write_CallInstance;
  if (Mock.spi_write_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_spi_write);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.spi_write_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
  call_instance = Mock.spi_read_CallInstance;
  if (Mock.spi_read_IgnoreBool)
    call_instance = CMOCK_GUTS_NONE;
  if (CMOCK_GUTS_NONE != call_instance)
  {
    UNITY_SET_DETAIL(CMockString_spi_read);
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLess);
  }
  if (Mock.spi_read_CallbackFunctionPointer != NULL)
  {
    call_instance = CMOCK_GUTS_NONE;
    (void)call_instance;
  }
}

void mock_spi_Init(void)
{
  mock_spi_Destroy();
}

void mock_spi_Destroy(void)
{
  CMock_Guts_MemFreeAll();
  memset(&Mock, 0, sizeof(Mock));
  GlobalExpectCount = 0;
  GlobalVerifyOrder = 0;
}

void spi_write(uint32_t blocksize, uint16_t* srcbuff)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_spi_write_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_spi_write);
  cmock_call_instance = (CMOCK_spi_write_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.spi_write_CallInstance);
  Mock.spi_write_CallInstance = CMock_Guts_MemNext(Mock.spi_write_CallInstance);
  if (Mock.spi_write_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    return;
  }
  if (!Mock.spi_write_CallbackBool &&
      Mock.spi_write_CallbackFunctionPointer != NULL)
  {
    Mock.spi_write_CallbackFunctionPointer(blocksize, srcbuff, Mock.spi_write_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (cmock_call_instance->CallOrder > ++GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledEarly);
  if (cmock_call_instance->CallOrder < GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLate);
  if (!cmock_call_instance->ExpectAnyArgsBool)
  {
  {
    UNITY_SET_DETAILS(CMockString_spi_write,CMockString_blocksize);
    UNITY_TEST_ASSERT_EQUAL_HEX32(cmock_call_instance->Expected_blocksize, blocksize, cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_spi_write,CMockString_srcbuff);
    if (cmock_call_instance->Expected_srcbuff == NULL)
      { UNITY_TEST_ASSERT_NULL(srcbuff, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_HEX16_ARRAY(cmock_call_instance->Expected_srcbuff, srcbuff, 1, cmock_line, CMockStringMismatch); }
  }
  }
  if (Mock.spi_write_CallbackFunctionPointer != NULL)
  {
    Mock.spi_write_CallbackFunctionPointer(blocksize, srcbuff, Mock.spi_write_CallbackCalls++);
  }
  UNITY_CLR_DETAILS();
}

void CMockExpectParameters_spi_write(CMOCK_spi_write_CALL_INSTANCE* cmock_call_instance, uint32_t blocksize, uint16_t* srcbuff)
{
  cmock_call_instance->Expected_blocksize = blocksize;
  cmock_call_instance->Expected_srcbuff = srcbuff;
}

void spi_write_CMockIgnore(void)
{
  Mock.spi_write_IgnoreBool = (char)1;
}

void spi_write_CMockStopIgnore(void)
{
  Mock.spi_write_IgnoreBool = (char)0;
}

void spi_write_CMockExpectAnyArgs(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_spi_write_CALL_INSTANCE));
  CMOCK_spi_write_CALL_INSTANCE* cmock_call_instance = (CMOCK_spi_write_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.spi_write_CallInstance = CMock_Guts_MemChain(Mock.spi_write_CallInstance, cmock_guts_index);
  Mock.spi_write_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  cmock_call_instance->ExpectAnyArgsBool = (char)1;
}

void spi_write_CMockExpect(UNITY_LINE_TYPE cmock_line, uint32_t blocksize, uint16_t* srcbuff)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_spi_write_CALL_INSTANCE));
  CMOCK_spi_write_CALL_INSTANCE* cmock_call_instance = (CMOCK_spi_write_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.spi_write_CallInstance = CMock_Guts_MemChain(Mock.spi_write_CallInstance, cmock_guts_index);
  Mock.spi_write_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_spi_write(cmock_call_instance, blocksize, srcbuff);
}

void spi_write_AddCallback(CMOCK_spi_write_CALLBACK Callback)
{
  Mock.spi_write_IgnoreBool = (char)0;
  Mock.spi_write_CallbackBool = (char)1;
  Mock.spi_write_CallbackFunctionPointer = Callback;
}

void spi_write_Stub(CMOCK_spi_write_CALLBACK Callback)
{
  Mock.spi_write_IgnoreBool = (char)0;
  Mock.spi_write_CallbackBool = (char)0;
  Mock.spi_write_CallbackFunctionPointer = Callback;
}

void spi_read(uint32_t blocksize, uint16_t* destbuff)
{
  UNITY_LINE_TYPE cmock_line = TEST_LINE_NUM;
  CMOCK_spi_read_CALL_INSTANCE* cmock_call_instance;
  UNITY_SET_DETAIL(CMockString_spi_read);
  cmock_call_instance = (CMOCK_spi_read_CALL_INSTANCE*)CMock_Guts_GetAddressFor(Mock.spi_read_CallInstance);
  Mock.spi_read_CallInstance = CMock_Guts_MemNext(Mock.spi_read_CallInstance);
  if (Mock.spi_read_IgnoreBool)
  {
    UNITY_CLR_DETAILS();
    return;
  }
  if (!Mock.spi_read_CallbackBool &&
      Mock.spi_read_CallbackFunctionPointer != NULL)
  {
    Mock.spi_read_CallbackFunctionPointer(blocksize, destbuff, Mock.spi_read_CallbackCalls++);
    UNITY_CLR_DETAILS();
    return;
  }
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringCalledMore);
  cmock_line = cmock_call_instance->LineNumber;
  if (cmock_call_instance->CallOrder > ++GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledEarly);
  if (cmock_call_instance->CallOrder < GlobalVerifyOrder)
    UNITY_TEST_FAIL(cmock_line, CMockStringCalledLate);
  if (!cmock_call_instance->ExpectAnyArgsBool)
  {
  {
    UNITY_SET_DETAILS(CMockString_spi_read,CMockString_blocksize);
    UNITY_TEST_ASSERT_EQUAL_HEX32(cmock_call_instance->Expected_blocksize, blocksize, cmock_line, CMockStringMismatch);
  }
  {
    UNITY_SET_DETAILS(CMockString_spi_read,CMockString_destbuff);
    if (cmock_call_instance->Expected_destbuff == NULL)
      { UNITY_TEST_ASSERT_NULL(destbuff, cmock_line, CMockStringExpNULL); }
    else
      { UNITY_TEST_ASSERT_EQUAL_HEX16_ARRAY(cmock_call_instance->Expected_destbuff, destbuff, 1, cmock_line, CMockStringMismatch); }
  }
  }
  if (Mock.spi_read_CallbackFunctionPointer != NULL)
  {
    Mock.spi_read_CallbackFunctionPointer(blocksize, destbuff, Mock.spi_read_CallbackCalls++);
  }
  UNITY_CLR_DETAILS();
}

void CMockExpectParameters_spi_read(CMOCK_spi_read_CALL_INSTANCE* cmock_call_instance, uint32_t blocksize, uint16_t* destbuff)
{
  cmock_call_instance->Expected_blocksize = blocksize;
  cmock_call_instance->Expected_destbuff = destbuff;
}

void spi_read_CMockIgnore(void)
{
  Mock.spi_read_IgnoreBool = (char)1;
}

void spi_read_CMockStopIgnore(void)
{
  Mock.spi_read_IgnoreBool = (char)0;
}

void spi_read_CMockExpectAnyArgs(UNITY_LINE_TYPE cmock_line)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_spi_read_CALL_INSTANCE));
  CMOCK_spi_read_CALL_INSTANCE* cmock_call_instance = (CMOCK_spi_read_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.spi_read_CallInstance = CMock_Guts_MemChain(Mock.spi_read_CallInstance, cmock_guts_index);
  Mock.spi_read_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  cmock_call_instance->ExpectAnyArgsBool = (char)1;
}

void spi_read_CMockExpect(UNITY_LINE_TYPE cmock_line, uint32_t blocksize, uint16_t* destbuff)
{
  CMOCK_MEM_INDEX_TYPE cmock_guts_index = CMock_Guts_MemNew(sizeof(CMOCK_spi_read_CALL_INSTANCE));
  CMOCK_spi_read_CALL_INSTANCE* cmock_call_instance = (CMOCK_spi_read_CALL_INSTANCE*)CMock_Guts_GetAddressFor(cmock_guts_index);
  UNITY_TEST_ASSERT_NOT_NULL(cmock_call_instance, cmock_line, CMockStringOutOfMemory);
  memset(cmock_call_instance, 0, sizeof(*cmock_call_instance));
  Mock.spi_read_CallInstance = CMock_Guts_MemChain(Mock.spi_read_CallInstance, cmock_guts_index);
  Mock.spi_read_IgnoreBool = (char)0;
  cmock_call_instance->LineNumber = cmock_line;
  cmock_call_instance->CallOrder = ++GlobalExpectCount;
  cmock_call_instance->ExpectAnyArgsBool = (char)0;
  CMockExpectParameters_spi_read(cmock_call_instance, blocksize, destbuff);
}

void spi_read_AddCallback(CMOCK_spi_read_CALLBACK Callback)
{
  Mock.spi_read_IgnoreBool = (char)0;
  Mock.spi_read_CallbackBool = (char)1;
  Mock.spi_read_CallbackFunctionPointer = Callback;
}

void spi_read_Stub(CMOCK_spi_read_CALLBACK Callback)
{
  Mock.spi_read_IgnoreBool = (char)0;
  Mock.spi_read_CallbackBool = (char)0;
  Mock.spi_read_CallbackFunctionPointer = Callback;
}

