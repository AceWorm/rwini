/**
 * @brief  Created by civen163@163.com
 * @date   2020-01-13
 * @author Civen
 */

#ifndef INI_OPTION_H
#define INI_OPTION_H

#ifdef __cplusplus
extern "C"
{
#endif

/*
    ;ini file format
    [session]
    key=value
*/

/** 
 * Get the specifed value form target file
 *  
 * @param file     Target INI file name
 * @param session  Session name
 * @param key      Key
 * 
 * @return char*   Value
 */
const char *iniGetValue( const char *file, const char *section, const char *key );

/**
 * Set the specifed value to target file 
 * 
 * @param file      Target INI file name
 * @param session   Session name
 * @param key       Key
 * @param value     new Value
 * 
 * @return int      0:success, -1:failed
 */
int iniSetValue( const char *file, const char *section, const char *key, const char *value );


/**
 * Flush INI data to disk file
 *  
 * @param file     Target INI file name
 * 
 * @return int     0:success, -1:failed
 */
int iniFlushData( const char *file );

#ifdef __cplusplus
}
#endif

#endif  // INI_OPTION_H
