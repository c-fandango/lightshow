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

def init_options(size, gpio_slowdown):
    options = RGBMatrixOptions()
    options.rows = size 
    options.cols = size 
    options.gpio_slowdown = gpio_slowdown
    options.hardware_mapping = 'regular'
    return options

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

def write_flag(value, flag_path):
    with open(flag_path,'w+') as file:
        file.write(value)

def run(sp, display_options, poll_interval_seconds, flag_path):
    image_url_prev = ''
    matrix = None 
    
    while True:
        time.sleep(poll_interval_seconds)
        try:
            playing_now = sp.currently_playing(additional_types='episode')
        except Exception:
            logging.exception('Poor Connection')
            continue
    
        if not playing_now:
            if matrix:
                matrix.Clear()
                image_url_prev, matrix = None, None
            logging.info('nothing playing')
           
            write_flag('0', flag_path)
            
        else:
            write_flag('1', flag_path)
    
            item = playing_now['item']
            image_url = item.get('album', item)['images'][2]['url']
        
            if image_url != image_url_prev:
    
                image = requests.get(image_url, stream=True)
    
                logging.info('changing image')
                image2 = Image.open(image.raw)
                matrix = RGBMatrix(options=display_options) 
                matrix.SetImage(image2.convert('RGB'))
            
                image_url_prev = image_url


parser = argparse.ArgumentParser(description='displays image of currently playing spotify song')
parser.add_argument('config_path')
args = parser.parse_args()

config = load_config(args.config_path)

logging.basicConfig(filename=config['log_file'], format='%(asctime)s - %(message)s', level=logging.INFO)

wait_for_connection(config['test_connection_url'])

options = init_options(**config['display'])

sp = init_spotify(**config['spotify'])

run(sp, options, **config['run'])


