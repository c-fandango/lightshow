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
options.gpio_slowdown=3

matrix=''

cid=''
secret= ''
uri='https://google.co.uk'
scope='user-read-currently-playing'

sp=spotipy.Spotify(auth_manager=SpotifyOAuth(client_id=cid,client_secret=secret,redirect_uri=uri, scope=scope, open_browser=False,cache_path='./.text'))
#print(sp.currently_playing())
print('authorised')
image_url_prev='.'
image_url=''
while True:
    try:
        status=sp.currently_playing()
        #print(status)
        status_pod=sp.currently_playing(additional_types='episode')
    except Exception as e:
        status = 'bad'
        matrix=''
        image_url=''
        print(e)
        print('Poor Connection')
        time.sleep(1)

    if status==None:
        if image_url != '':
            matrix.Clear()
            matrix=''
            image_url=''
            image_url_prev=''
        print('none')
       
        with open('./play_flag.txt','w') as pipe:
            pipe.write('0')
        time.sleep(2)
        
    elif status != 'bad':
        with open('./play_flag.txt','w') as pipe:
            pipe.write('1')
        time.sleep(1)
        if matrix=='':
            matrix=RGBMatrix(options=options)
        track_type=status['currently_playing_type']
        print(track_type)
        if track_type=='episode':
            image_url=status_pod['item']['images'][2]['url']
        elif track_type=='track':
            image_url=status['item']['album']['images'][2]['url']
    
        if image_url != image_url_prev:
            image=requests.get(image_url).content
            with open('./.album_art.jpg','wb') as file:
                file.write(image)
            print('changing image')
            image2=Image.open('./.album_art.jpg')
            matrix.SetImage(image2.convert('RGB'))
        
        time.sleep(2)
        image_url_prev=image_url

    


