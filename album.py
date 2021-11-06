from rgbmatrix import RGBMatrix, RGBMatrixOptions
import spotipy
from spotipy.oauth2 import  SpotifyOAuth
import requests
import time
from PIL import Image

disconnected=True

while disconnected:
    try:
        request=requests.get("https://www.google.co.uk/",timeout=5)
        print("Connected To Internet")
        disconnected=False
    except (requests.ConnectionError,requests.Timeout) as exception:
        print("No Internet")
        time.sleep(1)



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
image_url=''
while True:
    try:
        status=sp.currently_playing()
    except:
        status = 'bad'
        print('Poor Connection')
    if status==None:
        if image_url != '':
            matrix.Clear()
            matrix=''
            image_url=''
            image_url_prev=''
            print('none')
        time.sleep(5)
    elif status != 'bad':
        if matrix=='':
            matrix=RGBMatrix(options=options)
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

    


