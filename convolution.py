import numpy as np
import soundfile as sf

# functions ---------------------------------------------------------------------------------
def load_ir(path):
  ir = []
  f = open(path, 'r')
  for line in f :
    ir.append(float(line))
  f.close()

  return ir

def align_ir_rate(s_data, s_rate, ir_data, ir_rate) :
  # align ir's sampling rate
  aligned_ir = []
  si = 0
  for ii in range(len(ir_data)) :
    while si / s_rate < (ii + 1) / ir_rate :
      aligned_ir.append(ir_data[ii])
      si += 1

  return aligned_ir

def convolve(s_data, s_rate, ir_data, ir_rate) :
  output = []
  # align ir's sampling rate
  aligned_ir = align_ir_rate(s_data, s_rate, ir_data, ir_rate)

  for i in range(len(s_data)) :
    conv_comp = 0 
    for j in range(len(ir_data)) :
      if (i + j < len(s_data)) :
        conv_comp += s_data[i + j] * ir_data[j]
    output.append(conv_comp)
  
  return output

# execution ---------------------------------------------------------------------------------------------
path = "sound/coco.wav"
data, sample_rate = sf.read(path)

ir1 = load_ir('data/listener1.ir')
ir2 = load_ir('data/listener2.ir')
ir3 = load_ir('data/small_room.ir')
conv1 = convolve(data, sample_rate, ir1, 100)
conv2 = convolve(data, sample_rate, ir2, 100)
conv3 = convolve(data, sample_rate, ir3, 100)

sf.write('sound/coco1.wav', conv1, sample_rate)
sf.write('sound/coco2.wav', conv2, sample_rate)
sf.write('sound/coco3.wav', conv3, sample_rate)