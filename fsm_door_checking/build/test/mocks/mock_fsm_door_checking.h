/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCK_FSM_DOOR_CHECKING_H
#define _MOCK_FSM_DOOR_CHECKING_H

#include "unity.h"
#include "fsm_door_checking.h"

/* Ignore the following warnings, since we are copying code */
#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic push
#endif
#if !defined(__clang__)
#pragma GCC diagnostic ignored "-Wpragmas"
#endif
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wduplicate-decl-specifier"
#endif

void mock_fsm_door_checking_Init(void);
void mock_fsm_door_checking_Destroy(void);
void mock_fsm_door_checking_Verify(void);




#define fsm_door_checking_init_Ignore() fsm_door_checking_init_CMockIgnore()
void fsm_door_checking_init_CMockIgnore(void);
#define fsm_door_checking_init_StopIgnore() fsm_door_checking_init_CMockStopIgnore()
void fsm_door_checking_init_CMockStopIgnore(void);
#define fsm_door_checking_init_ExpectAnyArgs() fsm_door_checking_init_CMockExpectAnyArgs(__LINE__)
void fsm_door_checking_init_CMockExpectAnyArgs(UNITY_LINE_TYPE cmock_line);
#define fsm_door_checking_init_Expect(cmock_arg1, cmock_arg2) fsm_door_checking_init_CMockExpect(__LINE__, cmock_arg1, cmock_arg2)
void fsm_door_checking_init_CMockExpect(UNITY_LINE_TYPE cmock_line, fsm_door_checking_t* cmock_arg1, uint8_t cmock_arg2);
typedef void (* CMOCK_fsm_door_checking_init_CALLBACK)(fsm_door_checking_t* cmock_arg1, uint8_t cmock_arg2, int cmock_num_calls);
void fsm_door_checking_init_AddCallback(CMOCK_fsm_door_checking_init_CALLBACK Callback);
void fsm_door_checking_init_Stub(CMOCK_fsm_door_checking_init_CALLBACK Callback);
#define fsm_door_checking_init_StubWithCallback fsm_door_checking_init_Stub
#define fsm_door_checking_is_in_alarm_IgnoreAndReturn(cmock_retval) fsm_door_checking_is_in_alarm_CMockIgnoreAndReturn(__LINE__, cmock_retval)
void fsm_door_checking_is_in_alarm_CMockIgnoreAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define fsm_door_checking_is_in_alarm_StopIgnore() fsm_door_checking_is_in_alarm_CMockStopIgnore()
void fsm_door_checking_is_in_alarm_CMockStopIgnore(void);
#define fsm_door_checking_is_in_alarm_ExpectAnyArgsAndReturn(cmock_retval) fsm_door_checking_is_in_alarm_CMockExpectAnyArgsAndReturn(__LINE__, cmock_retval)
void fsm_door_checking_is_in_alarm_CMockExpectAnyArgsAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define fsm_door_checking_is_in_alarm_ExpectAndReturn(cmock_arg1, cmock_retval) fsm_door_checking_is_in_alarm_CMockExpectAndReturn(__LINE__, cmock_arg1, cmock_retval)
void fsm_door_checking_is_in_alarm_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, fsm_door_checking_t* cmock_arg1, uint8_t cmock_to_return);
typedef uint8_t (* CMOCK_fsm_door_checking_is_in_alarm_CALLBACK)(fsm_door_checking_t* cmock_arg1, int cmock_num_calls);
void fsm_door_checking_is_in_alarm_AddCallback(CMOCK_fsm_door_checking_is_in_alarm_CALLBACK Callback);
void fsm_door_checking_is_in_alarm_Stub(CMOCK_fsm_door_checking_is_in_alarm_CALLBACK Callback);
#define fsm_door_checking_is_in_alarm_StubWithCallback fsm_door_checking_is_in_alarm_Stub
#define fsm_door_checking_turn_on_off_active_Ignore() fsm_door_checking_turn_on_off_active_CMockIgnore()
void fsm_door_checking_turn_on_off_active_CMockIgnore(void);
#define fsm_door_checking_turn_on_off_active_StopIgnore() fsm_door_checking_turn_on_off_active_CMockStopIgnore()
void fsm_door_checking_turn_on_off_active_CMockStopIgnore(void);
#define fsm_door_checking_turn_on_off_active_ExpectAnyArgs() fsm_door_checking_turn_on_off_active_CMockExpectAnyArgs(__LINE__)
void fsm_door_checking_turn_on_off_active_CMockExpectAnyArgs(UNITY_LINE_TYPE cmock_line);
#define fsm_door_checking_turn_on_off_active_Expect(cmock_arg1, cmock_arg2) fsm_door_checking_turn_on_off_active_CMockExpect(__LINE__, cmock_arg1, cmock_arg2)
void fsm_door_checking_turn_on_off_active_CMockExpect(UNITY_LINE_TYPE cmock_line, fsm_door_checking_t* cmock_arg1, uint8_t cmock_arg2);
typedef void (* CMOCK_fsm_door_checking_turn_on_off_active_CALLBACK)(fsm_door_checking_t* cmock_arg1, uint8_t cmock_arg2, int cmock_num_calls);
void fsm_door_checking_turn_on_off_active_AddCallback(CMOCK_fsm_door_checking_turn_on_off_active_CALLBACK Callback);
void fsm_door_checking_turn_on_off_active_Stub(CMOCK_fsm_door_checking_turn_on_off_active_CALLBACK Callback);
#define fsm_door_checking_turn_on_off_active_StubWithCallback fsm_door_checking_turn_on_off_active_Stub

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif