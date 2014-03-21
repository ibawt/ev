local ev = {}

ev.app     = require 'app'
ev.stage   = require 'stage'
ev.sbatch  = require 'sbatch'.init(ev)
ev.texture = require 'texture'
ev.sprite  = require 'sprite'
ev.anim    = require 'anim'
return ev
