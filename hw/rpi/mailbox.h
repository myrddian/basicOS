/* 
 * File:   mailbox.h
 * Author: myrddian
 *
 * Created on 6 June 2014, 2:42 PM
 */

#ifndef MAILBOX_H
#define	MAILBOX_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct __mailboxbuffer {
        
        uint8_t channel;
        uint32_t value;
        
    }MailBoxBuffer;
    
extern const char mbox_name[];    
void registerMailBoxDriver();    

#ifdef	__cplusplus
}
#endif

#endif	/* MAILBOX_H */

