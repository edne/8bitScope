8bitScope
=========
Display a stream of audio bytes

Usage
-----
    your_audio_source | 8bitScope

for example taking input from [Jack](http://www.jackaudio.org/):

    jack-stdout -b8 0 1 | ./8bitScope
