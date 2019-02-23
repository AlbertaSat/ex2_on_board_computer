

#include "port.h"
#include "protocol_handler.h"


void packet_handler_server(Response res) {
    res.msg = "shutupa you\n";
    ssp_sendto(res);
}


void packet_handler_client(Response res) {
    res.msg = "noa youa shuta up!\n";
    ssp_sendto(res);

}