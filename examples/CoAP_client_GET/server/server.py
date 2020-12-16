#!/usr/bin/python

import netifaces as ni
from coapthon.server.coap import CoAP
from coapthon.resources.resource import Resource

class Hello(Resource):
    def __init__(self, name="hello", coap_server=None):
        super(Hello, self).__init__(name, coap_server, visible=True,
                                            observable=True, allow_children=True)
        self.payload = "hello from PC"
        self.content_type = "text/plain"

    def render_GET(self, request):
        return self

    
class CoAPServer(CoAP):
    def __init__(self, host, port, multicast=False):
        CoAP.__init__(self, (host, port), multicast)
        self.add_resource('hello', Hello())

        print ("CoAP Server start on {}:{}".format(host,str(port)))
        print (self.root.dump())


ni.ifaddresses('wlp2s0')
ip = ni.ifaddresses('wlp2s0')[ni.AF_INET][0]['addr']
server = CoAPServer(ip, 5683)

try:
    server.listen(10)
except KeyboardInterrupt:
    print ("Server Shutdown")
    server.close()
    print ("Exiting...")
