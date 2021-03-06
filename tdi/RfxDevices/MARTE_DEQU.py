from MDSplus import *
import time
import MARTE_GENERIC

class MARTE_DEQU(MARTE_GENERIC.MARTE_GENERIC):
    print 'MARTe DEQU'
    parNames = ['InputMapping', 'InputAutozero', 'bpCorrectionCoeffs', 'torI2TLimit', 'FluxDiffCorrectionOn', 'FluxDiffCorrectionCoeffs', 'AliasRemoveOn', 'TimeConstLpfMandF', 'FourthHarmRecOn', 'deltaTequ']
    parValues = [0,0, Data.compile('zero(32, 0.)+1.'), 0, 0, Data.compile('[-331.63E-9,-123.053E-9,-24.8494E-9,10.2545E-9,15.5055E-9,-35.3507E-9,-186.869E-9]'), 0, 0.16, 0, 0.05]
    parts = []
    for i in range(len(MARTE_GENERIC.MARTE_GENERIC.parts)):
      parts.append(MARTE_GENERIC.MARTE_GENERIC.parts[i])
    parts.append({'path':'.PARAMS', 'type':'structure'})
    parts.append({'path':'.PARAMS:NUM_ACTIVE', 'type':'numeric', 'value':len(parNames)})
    for i in range(len(parNames)):
      parts.append({'path':'.PARAMS:PAR_%03d'%(i+1), 'type':'structure'})
      parts.append({'path':'.PARAMS:PAR_%03d:DESCRIPTION'%(i+1), 'type':'text'})
      parts.append({'path':'.PARAMS:PAR_%03d:NAME'%(i+1), 'type':'text', 'value':parNames[i]})
      parts.append({'path':'.PARAMS:PAR_%03d:TYPE'%(i+1), 'type':'text'})
      parts.append({'path':'.PARAMS:PAR_%03d:DIMS'%(i+1), 'type':'numeric'})
      parts.append({'path':'.PARAMS:PAR_%03d:DATA'%(i+1), 'type':'numeric','value':parValues[i]})

    for i in range(len(parNames), 256):
      parts.append({'path':'.PARAMS:PAR_%03d'%(i+1), 'type':'structure'})
      parts.append({'path':'.PARAMS:PAR_%03d:DESCRIPTION'%(i+1), 'type':'text'})
      parts.append({'path':'.PARAMS:PAR_%03d:NAME'%(i+1), 'type':'text'})
      parts.append({'path':'.PARAMS:PAR_%03d:TYPE'%(i+1), 'type':'text'})
      parts.append({'path':'.PARAMS:PAR_%03d:DIMS'%(i+1), 'type':'numeric'})
      parts.append({'path':'.PARAMS:PAR_%03d:DATA'%(i+1), 'type':'numeric'})

    parts.append({'path':'.WAVE_PARAMS', 'type':'structure'})
    parts.append({'path':'.WAVE_PARAMS:NUM_ACTIVE', 'type':'numeric', 'value':0})
    for i in range(64):
      parts.append({'path':'.WAVE_PARAMS:WAVE_%03d'%(i+1), 'type':'structure'})
      parts.append({'path':'.WAVE_PARAMS:WAVE_%03d:DESCRIPTION'%(i+1), 'type':'text'})
      parts.append({'path':'.WAVE_PARAMS:WAVE_%03d:NAME'%(i+1), 'type':'text'})
      parts.append({'path':'.WAVE_PARAMS:WAVE_%03d:X'%(i+1), 'type':'numeric'})
      parts.append({'path':'.WAVE_PARAMS:WAVE_%03d:Y'%(i+1), 'type':'numeric'})

