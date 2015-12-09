#!/usr/bin/python

#Necesario dejarlo en /bin
#Se puede usar scp /MiCarpeta/ColorFB.py root@arduino.local:/bin/

import urllib2, json, datetime

access_token = ''#Token que acceso a FB
FotoID = ''#ID de la foto 

data = urllib2.urlopen('https://graph.facebook.com/' + FotoID + '?fields=comments.limit(1000)&access_token='+access_token).read()#optiene los ultimos 1000 comentarios de la FotoID
DatosFB = json.loads(data)#Transformar los datos a un areglo JSON

#datos Base 
Color = ""
Fecha =  datetime.datetime.strptime("2014-12-09T04:46:09+0000",'%Y-%m-%dT%H:%M:%S+0000') 
Nombre = ""

ColorActual = DatosFB['comments']['data'][0]['message']
FechaActual = datetime.datetime.strptime(DatosFB['comments']['data'][0]['created_time'],'%Y-%m-%dT%H:%M:%S+0000') 
NombreActual = DatosFB['comments']['data'][0]['from']['name']

for DatoActual in DatosFB['comments']['data']: #busqueda a encontrar el ultimo comentario 
   FechaActual = datetime.datetime.strptime(DatoActual['created_time'],'%Y-%m-%dT%H:%M:%S+0000')
   if FechaActual > Fecha:
        Fecha = FechaActual
        Color = DatoActual['message']
        Nombre = DatoActual['from']['name']

#Envia el color y el nombre del ultimo comentario
print Color 
print "@"+ Nombre


