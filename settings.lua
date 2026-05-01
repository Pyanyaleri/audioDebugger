-- Program Settings
program_name = "RoboÉireann audioDebugger"
font = "assets/fonts/JosefinSans-Regular.ttf"
font_size = 14
test_audio_file = "assets/audioSamples/shooting_star.flac"

-- Window Settings
window_w = 1600
window_h = 900

-- Audio Settings
audio_gain = 1
sampling_rate = 16000

-- Whistle Detector Model Settings
detection_delay_ms = 600
whistle_confidence_thresh = 0.7
buffer_size = 2048
fft_size = 256
fft_step = 128