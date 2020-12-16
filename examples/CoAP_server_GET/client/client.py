#!/usr/bin/python

from coapthon.client.helperclient import HelperClient

client = HelperClient(server=('192.168.1.102', 5683))
response = client.get('hello')
print(response.pretty_print())
client.stop()
