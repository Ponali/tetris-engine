engineVersion="1.1.0"

import random

getTimestamp = None
def initTetrisEngine(gt):
  global getTimestamp
  getTimestamp = gt

# NOTE: y positions are reversed on these tables: "-1" means 1 down, and not 1 up.
standardWallKicksCW=[
  [( 0, 0),(-1, 0),(-1,+1),( 0,-2),(-1,-2)],
  [( 0, 0),(+1, 0),(+1,-1),( 0,+2),(+1,+2)],
  [( 0, 0),(+1, 0),(+1,+1),( 0,-2),(+1,-2)],
  [( 0, 0),(-1, 0),(-1,-1),( 0,+2),(-1,+2)]
]
standardWallKicksCCW=[
  [( 0, 0),(+1, 0),(+1,+1),( 0,-2),(+1,-2)],
  [( 0, 0),(+1, 0),(+1,-1),( 0,+2),(+1,+2)],
  [( 0, 0),(-1, 0),(-1,+1),( 0,-2),(-1,-2)],
  [( 0, 0),(-1, 0),(-1,-1),( 0,+2),(-1,+2)]
]
standardWallKicks180=[ # from tetr.io
  [( 0, 0),( 0, 1),(+1,+1),(-1,+1),(+1, 0),(-1, 0)],
  [( 0, 0),(+1, 0),(+1,+2),(+1,+1),( 0,+2),( 0,+1)],
  [( 0, 0),( 0,-1),(-1,-1),(+1,-1),(-1, 0),(+1, 0)],
  [( 0, 0),(-1, 0),(-1,+2),(-1,+1),( 0,+2),( 0,+1)]
]
longPieceWallKicksCW=[
  [( 0, 0),(-2, 0),(+1, 0),(-2,-1),(+1,+2)],
  [( 0, 0),(-1, 0),(+2, 0),(-1,+2),(+2,-1)],
  [( 0, 0),(+2, 0),(-1, 0),(+2,+1),(-1,-2)],
  [( 0, 0),(+1, 0),(-2, 0),(+1,-2),(-2,+1)]
]
longPieceWallKicksCCW=[
  [( 0, 0),(-1, 0),(+2, 0),(-1,+2),(+2,-1)],
  [( 0, 0),(+2, 0),(-1, 0),(+2,+1),(-1,-2)],
  [( 0, 0),(+1, 0),(-2, 0),(+1,-2),(-2,+1)],
  [( 0, 0),(-2, 0),(+1, 0),(-2,-1),(+1,+2)]
]
longPieceWallKicks180=[ # i couldn't find anything on 180° rotations for I pieces, so this will be left unimplemented
  [( 0, 0)],
  [( 0, 0)],
  [( 0, 0)],
  [( 0, 0)]
]

tetrominoData=[[
  [[0,0,0,0],
   [1,1,1,1],
   [0,0,0,0],
   [0,0,0,0]],
  [[0,0,1,0],
   [0,0,1,0],
   [0,0,1,0],
   [0,0,1,0]],
  [[0,0,0,0],
   [0,0,0,0],
   [1,1,1,1],
   [0,0,0,0]],
  [[0,1,0,0],
   [0,1,0,0],
   [0,1,0,0],
   [0,1,0,0]]
],[
  [[0,1,1,0],
   [0,1,1,0],
   [0,0,0,0],
   [0,0,0,0]],
  [[0,1,1,0],
   [0,1,1,0],
   [0,0,0,0],
   [0,0,0,0]],
  [[0,1,1,0],
   [0,1,1,0],
   [0,0,0,0],
   [0,0,0,0]],
  [[0,1,1,0],
   [0,1,1,0],
   [0,0,0,0],
   [0,0,0,0]]
],[
  [[0,1,0,0],
   [1,1,1,0],
   [0,0,0,0],
   [0,0,0,0]],
  [[0,1,0,0],
   [0,1,1,0],
   [0,1,0,0],
   [0,0,0,0]],
  [[0,0,0,0],
   [1,1,1,0],
   [0,1,0,0],
   [0,0,0,0]],
  [[0,1,0,0],
   [1,1,0,0],
   [0,1,0,0],
   [0,0,0,0]]
],[
  [[0,0,1,0],
   [1,1,1,0],
   [0,0,0,0],
   [0,0,0,0]],
  [[0,1,0,0],
   [0,1,0,0],
   [0,1,1,0],
   [0,0,0,0]],
  [[0,0,0,0],
   [1,1,1,0],
   [1,0,0,0],
   [0,0,0,0]],
  [[1,1,0,0],
   [0,1,0,0],
   [0,1,0,0],
   [0,0,0,0]]
],[
  [[1,0,0,0],
   [1,1,1,0],
   [0,0,0,0],
   [0,0,0,0]],
  [[0,1,1,0],
   [0,1,0,0],
   [0,1,0,0],
   [0,0,0,0]],
  [[0,0,0,0],
   [1,1,1,0],
   [0,0,1,0],
   [0,0,0,0]],
  [[0,1,0,0],
   [0,1,0,0],
   [1,1,0,0],
   [0,0,0,0]]
],[
  [[0,1,1,0],
   [1,1,0,0],
   [0,0,0,0],
   [0,0,0,0]],
  [[0,1,0,0],
   [0,1,1,0],
   [0,0,1,0],
   [0,0,0,0]],
  [[0,0,0,0],
   [0,1,1,0],
   [1,1,0,0],
   [0,0,0,0]],
  [[1,0,0,0],
   [1,1,0,0],
   [0,1,0,0],
   [0,0,0,0]]
],[
  [[1,1,0,0],
   [0,1,1,0],
   [0,0,0,0],
   [0,0,0,0]],
  [[0,0,1,0],
   [0,1,1,0],
   [0,1,0,0],
   [0,0,0,0]],
  [[0,0,0,0],
   [1,1,0,0],
   [0,1,1,0],
   [0,0,0,0]],
  [[0,1,0,0],
   [1,1,0,0],
   [1,0,0,0],
   [0,0,0,0]]
]]

class Grid2D:
  def __init__(s,cols,rows):
    s.arr=[0 for i in range(cols*rows)]
    s.cols=cols
    s.rows=rows
  def get(s,x,y,replace=None):
    if x<0 or y<0 or x>=s.cols or y>=s.rows:
      if replace!=None:
        return replace
      raise IndexError("2D list index out of range (%d,%d)"%(x,y))
    return s.arr[x+y*s.cols]
  def set(s,x,y,v):
    if x<0 or y<0 or x>=s.cols or y>=s.rows:
      raise IndexError("2D list index out of range (%d,%d)"%(x,y))
    s.arr[x+y*s.cols]=v
  def copy(s):
    n=Grid2D(s.cols,s.rows)
    n.arr=s.arr.copy()
    return n

class TetrisBag:
  def __init__(s):
    s.outgoing = []
    s.addRandomPieces()
    s.addRandomPieces()
  def addRandomPieces(s):
    pieceList = [i+random.randint(0,1)*7 for i in range(7)]
    pieceList = sorted(pieceList, key=lambda x: random.random())
    s.outgoing.extend(pieceList)
  def pop(s):
    piece = s.outgoing.pop(0)
    if len(s.outgoing)==7:
      s.addRandomPieces()
    return piece

# TODO: make gravity be able to do >1G up to 20G
# TODO: add *-Spins (if they exist)
# TODO: add optional support for arika SRS
# TODO: make lock delay customizable
# TODO: make gravity speed customizable
# TODO: pass invisible row count, and spawn piece Y based on that instead of hardcoded 4
class TetrisGrid:
  def __init__(s,cols,rows,spawnHeight,eventHandler):
    s.grid=Grid2D(cols,rows)
    s.bag=TetrisBag()
    s.gravityUpdate = getTimestamp()
    s.pieceLock = False
    s.pieceLockTimestamp = 0
    s.pieceLockResetMoves = 15
    s.pieceTSpin = False
    s.cols=cols
    s.rows=rows
    s.spawnPiecesRotated=False
    s.spawnHeight=spawnHeight
    s.eventHandler=eventHandler
    s.linesCleared = 0
    s.levelLines = 0
    s.level = 1
    s.score = 0
    s.combo = 0
    s.paused = False
    s.pauseTimestamp = 0
    s.backToBack = 0
    s.holdPiece = None
    s.canHoldPiece = True
    s.gameOver = False
    # s.spawnTetrimino()
  def getGravitySpeed(s):
    return (800-((s.level-1)*7))
    # formula from Tetris Worlds: https://harddrop.com/wiki/Tetris_Worlds -> Gravity
  def getLockDelayTime(s):
    return 500 # the lock delay doesn't change, and is always set to be half a second
  def tetriminoColliding(s,x,y,t,r,ground=False):
    d=tetrominoData[t][r]
    for i in range(4):
      for j in range(4):
        if d[i][j]==1:
          vx=j+x
          vy=i+y
          if vx<0 or vy<0 or vx>=s.cols or vy>=s.rows:
            if ground:
              return vy>=s.rows
            else:
              return True
          elif s.grid.get(vx,vy)>0:
            return True
    return False
  def spawnSpecificTetrimino(s,tt):
    # s.tx=3
    s.tx=(s.cols-4)//2
    s.ty=s.spawnHeight
    s.tt=tt
    s.tr=0
    if s.tt>=7:
      s.tt-=7
      # make piece spawn 180° with s.ty 1 less
      if s.spawnPiecesRotated:
        s.ty=3
        s.tr=2
  def spawnTetrimino(s):
    s.spawnSpecificTetrimino(s.bag.pop())
    s.pieceLock = False
    s.pieceLockTimestamp = 0
    s.pieceTSpin = False
    if s.tetriminoColliding(s.tx,s.ty,s.tt,s.tr):
      s.gameOver = True
    s.pieceLockResetMoves = 15
    s.checkPieceLock()
    if not s.canHoldPiece:
      s.canHoldPiece = True
      s.eventHandler(s,"updateHoldPiece",{})
    s.eventHandler(s,"updateNext",{})
  def checkPieceLock(s):
    if s.tetriminoColliding(s.tx,s.ty+1,s.tt,s.tr,True):
      if not s.pieceLock:
        s.pieceLock = True
        s.pieceLockTimestamp = getTimestamp()
      else:
        s.pieceLockResetMoves-=1
        if s.pieceLockResetMoves>0:
          s.pieceLockTimestamp = getTimestamp()
    else: s.pieceLock = False
  def checkTSpin(s):
    if s.tt!=2: return
    if s.grid.get(s.tx,s.ty,1)>0 or s.grid.get(s.tx+2,s.ty,1)>0: # top part
      if s.grid.get(s.tx,s.ty+2,1)>0 and s.grid.get(s.tx+2,s.ty+2,1)>0: # bottom part
        s.pieceTSpin = True
  def isMiniTSpin(s):
    if s.tt!=2: return False
    if s.tr==0: return not (s.grid.get(s.tx+0,s.ty+0)>0 and s.grid.get(s.tx+2,s.ty+0)>0)
    if s.tr==1: return not (s.grid.get(s.tx+2,s.ty+0)>0 and s.grid.get(s.tx+2,s.ty+2)>0)
    if s.tr==2: return not (s.grid.get(s.tx+2,s.ty+2)>0 and s.grid.get(s.tx+0,s.ty+2)>0)
    if s.tr==3: return not (s.grid.get(s.tx+0,s.ty+2)>0 and s.grid.get(s.tx+0,s.ty+0)>0)
  def holdCurrentPiece(s):
    if not s.canHoldPiece:
      return
    heldPiece = s.holdPiece
    s.holdPiece = s.tt
    if heldPiece!=None:
      s.spawnSpecificTetrimino(heldPiece)
    else:
      s.spawnTetrimino()
    s.canHoldPiece = False
    s.eventHandler(s,"updateHoldPiece",{})
  def movePiece(s,vx,vy):
    if not s.tetriminoColliding(s.tx+vx,s.ty+vy,s.tt,s.tr):
      s.tx+=vx
      s.ty+=vy
      s.checkPieceLock()
      return True
    else: return False
  def rotatePieceCW(s): # Clockwise
    newr=(s.tr+1)%4
    kickData = standardWallKicksCW[s.tr]
    if s.tt==0:
      kickData = longPieceWallKicksCW[s.tr]
    for i in range(len(kickData)):
      if not s.tetriminoColliding(s.tx+kickData[i][0],s.ty-kickData[i][1],s.tt,newr):
        s.tx+=kickData[i][0]
        s.ty-=kickData[i][1]
        s.tr=newr
        s.checkPieceLock()
        s.checkTSpin()
        return
  def rotatePieceCCW(s): # Counter clockwise
    newr=(s.tr+3)%4
    kickData = standardWallKicksCCW[s.tr]
    if s.tt==0:
      kickData = longPieceWallKicksCCW[s.tr]
    for i in range(len(kickData)):
      if not s.tetriminoColliding(s.tx+kickData[i][0],s.ty-kickData[i][1],s.tt,newr):
        s.tx+=kickData[i][0]
        s.ty-=kickData[i][1]
        s.tr=newr
        s.checkPieceLock()
        s.checkTSpin()
        return
  def rotatePiece180(s): # Rotate 180°
    newr=(s.tr+2)%4
    kickData = standardWallKicks180[s.tr]
    if s.tt==0:
      kickData = longPieceWallKicks180[s.tr]
    for i in range(len(kickData)):
      if not s.tetriminoColliding(s.tx+kickData[i][0],s.ty-kickData[i][1],s.tt,newr):
        s.tx+=kickData[i][0]
        s.ty-=kickData[i][1]
        s.tr=newr
        s.checkPieceLock()
        s.checkTSpin()
        return
  def isAllClear(s):
    for i in range(s.rows):
      for j in range(s.cols):
        if s.grid.get(j,i)>0:
          return False
    return True
  def scoreClearedLines(s,linesFound,isMiniTSpin):
    # handle combo
    if linesFound>0:
      s.combo+=1
    else:
      s.combo=0
    if s.combo>=2:
      s.score+=50*(s.combo-1)*s.level
    # handle back-to-back
    if linesFound>0:
      if linesFound<4 and not s.pieceTSpin:
        s.backToBack=0
      else:
        s.backToBack+=1
    b2bMultiplier=1
    if s.backToBack>2:
      b2bMultiplier=1.5
    # handle "all clear"
    allClear=s.isAllClear()
    # add points to score
    if allClear:
      if s.backToBack>2 and linesFound>=4:
        s.score+=3200*s.level
      else:
        if linesFound==1: s.score+=800*s.level*b2bMultiplier
        if linesFound==2: s.score+=1200*s.level*b2bMultiplier
        if linesFound==3: s.score+=1800*s.level*b2bMultiplier
        if linesFound==4: s.score+=2000*s.level*b2bMultiplier
    else:
      if s.pieceTSpin:
        if isMiniTSpin:
          if linesFound==0: s.score+=100*s.level
          if linesFound==1: s.score+=200*s.level*b2bMultiplier
          if linesFound==2: s.score+=400*s.level*b2bMultiplier
        else:
          if linesFound==0: s.score+=400*s.level
          if linesFound==1: s.score+=800*s.level*b2bMultiplier
          if linesFound==2: s.score+=1200*s.level*b2bMultiplier
          if linesFound==3: s.score+=1600*s.level*b2bMultiplier
      else:
        if linesFound==1: s.score+=100*s.level*b2bMultiplier
        if linesFound==2: s.score+=300*s.level*b2bMultiplier
        if linesFound==3: s.score+=500*s.level*b2bMultiplier
        if linesFound==4: s.score+=800*s.level*b2bMultiplier
    # update level
    while s.levelLines>10:
      s.level+=1
      s.levelLines=s.levelLines-10
    # send t-spin reward
    if s.pieceTSpin:
      s.eventHandler(s,"tSpin",{"lines":linesFound,"mini":isMiniTSpin})
    elif linesFound>0:
      s.eventHandler(s,"lineClear",{"lines":linesFound,"allClear":allClear})
    s.eventHandler(s,"updateScore",{})
  def clearLines(s):
    isMiniTSpin = s.isMiniTSpin()
    linesFound=0
    def removeLine(y):
      nonlocal linesFound # wtf is that keyword??
      # add to score
      s.linesCleared+=1
      s.levelLines+=1
      linesFound+=1
      # copy
      for i in range(y-1,-1,-1):
        for j in range(s.cols):
          s.grid.set(j,i+1,s.grid.get(j,i))
      # remove top line
      for j in range(s.cols):
        s.grid.set(j,0,0)
    def clearCheck(y):
      lineClear = True
      for x in range(s.cols):
        if s.grid.get(x,y)==0:
          lineClear = False
      return lineClear
    linesToClear=[]
    for y in range(s.rows):
      if clearCheck(y):
        linesToClear.append(y)
    if len(linesToClear)>0:
      s.eventHandler(s,"lineClearAnim",{"lines":linesToClear})
    while len(linesToClear)>0:
      y=linesToClear.pop(0)
      removeLine(y)
    s.scoreClearedLines(linesFound,isMiniTSpin)
  def placeCurrentPiece(s):
    s.eventHandler(s,"place",{})
    d=tetrominoData[s.tt][s.tr]
    for i in range(4):
      for j in range(4):
        if d[i][j]==1:
          s.grid.set(j+s.tx,i+s.ty,s.tt+1)
    s.clearLines()
    s.spawnTetrimino()
  def dropCurrentPiece(s,place):
    amount=0
    while s.movePiece(0,1):
      amount+=1
    if place: s.placeCurrentPiece()
    if place:
      s.score+=2*amount
    else:
      s.score+=1*amount
    s.eventHandler(s,"updateScore",{})
  def getHardDropYPosition(s):
    for i in range(s.ty,s.rows):
      if s.tetriminoColliding(s.tx,i,s.tt,s.tr):
        return i-1
  def getScreenWithCurrentPiece(s):
    scr=s.grid.copy()
    d=tetrominoData[s.tt][s.tr]
    shadowY=s.getHardDropYPosition()
    for i in range(4):
      for j in range(4):
        if d[i][j]==1:
          scr.set(j+s.tx,i+shadowY,8) # shadow
          scr.set(j+s.tx,i+s.ty,s.tt+1)
    return scr
  def cellOnCurrentPiece(s,x,y):
    d=tetrominoData[s.tt][s.tr]
    vx=x-s.tx
    vy=y-s.ty
    if vx<0 or vy<0 or vx>=4 or vy>=4:
      return False
    return d[vy][vx]==1
  def pause(s,pauseValue=None):
    if pauseValue==None:
      pauseValue=not s.paused
    if s.paused!=pauseValue:
      s.paused=pauseValue
      if pauseValue:
        s.pauseTimestamp=getTimestamp()
      else:
        pauseTime=getTimestamp()-s.pauseTimestamp
        s.gravityUpdate+=pauseTime
        s.pieceLockTimestamp+=pauseTime
        # NOTE: if any more timestamps show up, make sure to put them here
  def update(s,left,right,down,rotateCW,rotateCCW,rotate180,sonicDrop,hardDrop,holdPiece):
    if s.gameOver: return
    left=int(left)
    right=int(right)
    down=int(down)
    if sonicDrop:     s.dropCurrentPiece(False)
    elif hardDrop:    s.dropCurrentPiece(True)
    elif left>0:
      for i in range(left):
        s.movePiece(-1, 0)
    elif right>0:
      for i in range(right):
        s.movePiece( 1, 0)
    elif down>0:
      for i in range(down):
        s.movePiece( 0, 1)
      s.score+=1*down
      s.eventHandler(s,"updateScore",{})
    elif rotateCW:    s.rotatePieceCW()
    elif rotateCCW:   s.rotatePieceCCW()
    elif rotate180:   s.rotatePiece180()
    elif holdPiece:   s.holdCurrentPiece()
    if s.gravityUpdate+s.getGravitySpeed()<getTimestamp():
      s.gravityUpdate=getTimestamp()
      s.movePiece( 0, 1)
    if s.pieceLock and s.pieceLockTimestamp+s.getLockDelayTime()<getTimestamp():
      s.placeCurrentPiece()
