import time
import yaml
import logging
import spotipy
import argparse
import requests
from PIL import Image
from spotipy.oauth2 import SpotifyOAuth
from rgbmatrix import RGBMatrix, RGBMatrixOptions

def load_config(path):
    with open(path, 'r') as file:
        return yaml.safe_load(file) 

def wait_for_connection(url):
    while True:
        try:
            return requests.get(url,timeout=5)
        except (requests.ConnectionError,requests.Timeout) as exception:
            logging.exception('Failed to establish internet connection')
            time.sleep(1)

def init_display(size, gpio_slowdown):
    options = RGBMatrixOptions()
    options.rows = size 
    options.cols = size 
    options.gpio_slowdown = gpio_slowdown
    options.hardware_mapping = 'regular'

    return RGBMatrix(options=options)

def init_spotify(access_id, secret, redirect_uri, scope, cache):
    auth_manager = SpotifyOAuth(
        client_id=access_id,
        client_secret = secret,
        redirect_uri = redirect_uri,
        scope = scope,
        open_browser = False,
        cache_path = cache
    )
    return spotipy.Spotify(auth_manager=auth_manager)

parser = argparse.ArgumentParser(description='displays image of currently playing spotify song')
parser.add_argument('config_path')
args = parser.parse_args()

config = load_config(args.config_path)

logging.basicConfig(filename=config['log_file'], format='%(asctime)s - %(message)s', level=logging.INFO)


wait_for_connection(config['test_connection_url'])

matrix = init_display(**config['display'])

sp = init_spotify(**config['spotify'])

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
