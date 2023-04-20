#include "parser.h"
#include "common.h"
#include <algorithm>

void Parser::parseMessage(char *_message, size_t _len)
{
    m_state = Head;
    uint32_t messageLength = 0;
    int lenOfFieldMessage = 4;
    static int position = 0;
    for(size_t ind = 0; ind < _len; ++ind){
        switch (m_state) {

        case Head :{
            if(_message[ind] == 'Q'){
                m_state = Length;
            }
            break;
        }

        case Length : {
            if(m_state == Length){
                auto fieldAsBytes = std::vector<char>(_message+ind,_message+ind+lenOfFieldMessage);
                std::reverse(fieldAsBytes.begin(),fieldAsBytes.end());
                messageLength = *reinterpret_cast<uint32_t*>(&fieldAsBytes[0]);
                std::cout<<"length of incomming informational message "<<messageLength<<std::endl;
                ind+=lenOfFieldMessage-1;
                m_state = Message;
            }

            break;
        }

        case Message : {
            char message[messageLength];
            strncpy(message,&_message[ind],messageLength);
            LOG()<<message;
            m_state = Head;

        }
            break;
        }
    }
}
