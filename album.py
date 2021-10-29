from typing import Type
from rgbmatrix import RGBMatrix, RGBMatrixOptions
import spotipy
from spotipy.oauth2 import  SpotifyOAuth
import requests
import time
from PIL import Image

options=RGBMatrixOptions()
options.rows=64
options.cols=64
options.hardware_mapping = 'regular'


matrix=RGBMatrix(options=options)

cid='080f0dda7db742c5ba6d3fa8dd246067'
secret= '5fc96e36afa041618def8e87956a1649'
uri='https://google.co.uk'
scope='user-read-currently-playing'
'''tester'''

sp=spotipy.Spotify(auth_manager=SpotifyOAuth(client_id=cid,client_secret=secret,redirect_uri=uri, scope=scope, open_browser=False,cache_path='/home/pi/Code/light-show/text'))
#print(sp.currently_playing())
print('authorised')
image_url_prev='.'

while True:
    status=sp.currently_playing()
    if status==None:
        matrix.Clear()
        image_url=''
        time.sleep(5)
    else:
        track_type=sp.currently_playing()['currently_playing_type']
        print(track_type)
        if track_type=='episode':
            image_url=sp.currently_playing(additional_types='episode')['item']['images'][2]['url']
        elif track_type=='track':
            image_url=sp.currently_playing()['item']['album']['images'][2]['url']


    
        if image_url != image_url_prev:
            image=requests.get(image_url).content
            with open('/home/pi/Code/light-show/album_art.jpg','wb') as file:
                file.write(image)
            image2=Image.open('/home/pi/Code/light-show/album_art.jpg')
            matrix.SetImage(image2.convert('RGB'))

        
        time.sleep(4)
        image_url_prev=image_url

    


