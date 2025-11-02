frontendVersion="1.0.0"

import random
import time
def getTimestamp():
  return time.time()*1000
from engine import initTetrisEngine, Grid2D, TetrisGrid, tetrominoData, engineVersion
initTetrisEngine(getTimestamp)

import os, sys, signal, tty, termios, select, shutil

columns = 10
rows = 20
invisibleRows = 4
spawnPiecesRotated = False
elektronikaMode = False
fastRenderMode = False

for i,v in enumerate(sys.argv):
  if i==0: continue
  if "=" in v:
    name=v[0:v.index("=")]
    value=v[v.index("=")+1:]
    match name:
      case "w" | "width" | "c" | "cols" | "columns":
        columns=int(value)
      case "h" | "height" | "r" | "rows" | "l" | "lines":
        rows=int(value)
      case "ir" | "irows" | "bz" | "bufferzone":
        invisibleRows=int(value)
  else: # "garry" and "russian"
    match v:
      case "sr" | "rotate" | "spawnrotated":
        spawnPiecesRotated=True
      case "alexey" | "leonidovich" | "pajitnov" | "elektronika":
        elektronikaMode=True
      case "fast" | "fastrender" | "boxrender":
        fastRenderMode=True

rewardColor=(255,255,255)
rewardString=""
rewardTimestamp=0
rewardClear=False

linesCleared=[False for i in range(rows+invisibleRows)]
lineClearTimestamp=0

def eventHandler(_,eventType,data):
  global rewardClear,rewardColor,rewardString,rewardTimestamp
  global linesCleared,lineClearTimestamp,gridBefore
  if eventType=="place":
    renderGrid(False)
  if eventType=="updateNext" or eventType=="updateScore":
    renderSide()
  if eventType=="updateHoldPiece":
    renderHoldPiece()
  if eventType=="tSpin":
    rewardString=["t-spin","t-spin single","t-spin double","t-spin triple"][data["lines"]]
    rewardColor=(255,80,255)
    if data["mini"]:
      rewardString=f"mini {rewardString}"
      rewardColor=(210,170,210)
    rewardTimestamp=getTimestamp()
    rewardClear=True
  if eventType=="lineClearAnim":
    linesCleared=[False for i in range(rows+invisibleRows)]
    for y in data["lines"]:
      linesCleared[y]=True
    lineClearTimestamp=getTimestamp()
    gridBefore=None
  if eventType=="lineClear":
    if data["allClear"]:
      rewardString="ALL CLEAR!"
      rewardColor=(255,255,190)
    else:
      match data["lines"]:
        case 1: return
        case 2:
          rewardString="double"
          rewardColor=(80,80,80)
        case 3:
          rewardString="triple"
          rewardColor=(150,180,180)
        case 4:
          rewardString="tetris"
          rewardColor=(80,255,255)
    rewardTimestamp=getTimestamp()
    rewardClear=True

grid = TetrisGrid(columns,rows+invisibleRows,spawnPiecesRotated,eventHandler)

stdoutFileno=sys.stdout.fileno()
def writeToStdout(string):
  os.write(stdoutFileno,string.encode("utf-8"))

def getCellColor(cell):
  if elektronikaMode:
    return (0,255,0)
  match cell:
    case 0: return (  0,  0,  0)
    case 1: return (  0,255,255)
    case 2: return (255,255,  0)
    case 3: return (255,  0,255)
    case 4: return (255,127,  0)
    case 5: return (  0,  0,255)
    case 6: return (  0,255,  0)
    case 7: return (255,  0,  0)
    case 8: return ( 80, 80, 80)

def doubleSize(string):
  return f"\x1b#3{string}\n\x1b#4\r{string}"

def center(string,res):
  return " "*round((res-len(string))/2)+string

def getGridPos():
  gridX = (ttySize.columns-columns*2)//2
  gridY = (ttySize.lines-rows)//2
  return (gridX,gridY)

# TODO: add playfield border

gridBefore = None
def renderGrid(usePieceDim):
  global gridBefore
  gridX,gridY=getGridPos()
  pieceDim = 1
  if grid.pieceLock:
    timePassed = (getTimestamp()-grid.pieceLockTimestamp)/grid.getLockDelayTime()
    pieceDim = 1-timePassed/2
  scr=grid.getScreenWithCurrentPiece()
  if elektronikaMode:
    writeToStdout("\x1b[38;2;0;255;0m")
  for y in range(invisibleRows,rows+invisibleRows):
    for x in range(columns):
      cell=scr.get(x,y)
      if (gridBefore!=None and gridBefore.get(x,y)==cell) and not (grid.cellOnCurrentPiece(x,y) or (linesCleared[y] and getTimestamp()-lineClearTimestamp<600)):
        continue
      writeToStdout(f"\x1b[{gridY+y-invisibleRows};{gridX+x*2}f")
      if elektronikaMode:
        if cell==0 or cell==8:
          writeToStdout(" .")
        else:
          writeToStdout("[]")
      else:
        r,g,b = getCellColor(cell)
        if grid.cellOnCurrentPiece(x,y) and usePieceDim:
          r=r*pieceDim
          g=g*pieceDim
          b=b*pieceDim
        if linesCleared[y]:
          light=max(min(1.2-(getTimestamp()-lineClearTimestamp)/500,1),0)
          r=max(min(r+light*70,255),0)
          g=max(min(g+light*70,255),0)
          b=max(min(b+light*70,255),0)
        dr=max(min(r-15,255),0)
        dg=max(min(g-15,255),0)
        db=max(min(b-15,255),0)
        br=max(min(r+60,255),0)
        bg=max(min(g+60,255),0)
        bb=max(min(b+60,255),0)
        if cell==0 or cell==8 or fastRenderMode:
          writeToStdout(f"\x1b[48;2;{int(r)};{int(g)};{int(b)}m  ")
        else:
          writeToStdout(f"\x1b[48;2;{int(r)};{int(g)};{int(b)}m\x1b[38;2;{int(dr)};{int(dg)};{int(db)}m🬿\x1b[38;2;{int(br)};{int(bg)};{int(bb)}m🭥")
    # writeToStdout(f"\x1b[{columns*2}D\x1b[B")
  # writeToStdout(f"\x1b[{rows}A")
  gridBefore=scr

def renderPiece(piece,col=None):
  rotation=0
  if piece>=7:
    piece-=7
    if spawnPiecesRotated:
      rotation=2
  d=tetrominoData[piece][rotation]
  r,g,b = getCellColor(piece+1)
  if col!=None:
    writeToStdout(f"\x1b[38;2;{col[0]};{col[1]};{col[2]}m")
  else:
    writeToStdout(f"\x1b[38;2;{r};{g};{b}m")
  blocks = [" ","▀","▄","█"]
  for i in range(0,4,2):
    for j in range(4):
      writeToStdout(blocks[d[i][j]+d[i+1][j]*2])
    writeToStdout("\x1b[4D\n")

ttySize = shutil.get_terminal_size()
def showInfo():
  writeToStdout(f"\x1b[{ttySize.lines};1f\x1b#5\x1b[2m\x1b[2K")
  text=f"tetris-engine v{engineVersion} terminal frontend v{frontendVersion} (python)"
  writeToStdout(text+" "*(ttySize.columns-len(text)))
  writeToStdout("\x1b[0m")

def renderReward():
  global rewardColor
  if elektronikaMode:
    rewardColor=(0,255,0)
  posY = min((ttySize.lines+rows+2)//2,ttySize.lines-2)
  writeToStdout(f"\x1b[{posY};1f")
  fade=max(min(2.5-(getTimestamp()-rewardTimestamp)/600,1),0)
  r=fade*rewardColor[0]
  g=fade*rewardColor[1]
  b=fade*rewardColor[2]
  writeToStdout(f"\x1b[49m\x1b[38;2;{int(r)};{int(g)};{int(b)}m")
  if fade>0:
    ansiDim=""
    if fade<0.5:
      ansiDim="\x1b[2m"
    writeToStdout(doubleSize("\x1b[2K"+ansiDim+center(rewardString,ttySize.columns//2)))
  else:
    writeToStdout(doubleSize("\x1b[2K"))
    rewardClear=False
def renderSide():
  gridX,gridY=getGridPos()
  writeToStdout(f"\x1b[0m\x1b[{gridY};{gridX+columns*2+1}f")
  for i in range(4):
    renderPiece(grid.bag.outgoing[i])
  if elektronikaMode:
    writeToStdout("\x1b[38;2;0;255;0m")
  else:
    writeToStdout("\x1b[39;49m")
  writeToStdout("\x1b[s")
  clearLine="\x1b[K"
  string=f"level {grid.level}\nlines: {grid.linesCleared}\nscore: {"%06d"%grid.score}"
  if grid.combo>=2:
    string+=f"\n\x1b[0m\x1b[1mcombo x{grid.combo}\x1b[22m"
    # turns out, using "0m" instead of "39m" is required to have the plasma-themed bold color
  if grid.backToBack>=2:
    string+=f"\n\x1b[0m\x1b[1mback-to-back x{grid.backToBack}\x1b[22m"
  string+="\n\n"
  writeToStdout(clearLine+string.replace("\n","\x1b[u\n\x1b[s"+clearLine))
  sys.stdout.flush()
def renderHoldPiece():
  gridX,gridY=getGridPos()
  writeToStdout("\x1b[39m\x1b[48;2;50;50;50m")
  if elektronikaMode:
    writeToStdout("\x1b[49m\x1b[38;2;0;255;0m")
  writeToStdout(f"\x1b[{gridY};{gridX-6}f")
  writeToStdout((" "*6+"\n\x1b[6D")*3+"\x1b[3A")
  writeToStdout(" HOLD\n\x1b[4D")
  if grid.holdPiece==None:
    writeToStdout("\x1b[38;2;100;100;100mnone")
  else:
    col=None
    if not grid.canHoldPiece:
      col=(120,120,120)
    renderPiece(grid.holdPiece,col)
  writeToStdout("\x1b[39;49m")
  sys.stdout.flush()
def render():
  gridX,gridY=getGridPos()
  # writeToStdout(f"\x1b[{gridY};{gridX}f")
  renderGrid(True)
  if rewardClear:
    renderReward()
  sys.stdout.flush()
def rerender():
  global gridBefore
  writeToStdout("\x1b[0m\x1b[H\x1b[2J") # clear screen
  showInfo()
  gridBefore=None
  renderGrid(True)
  rewardClear=True
  renderReward()
  renderSide()
  renderHoldPiece()
def resChange(signum,frame):
  global ttySize,gridBefore,rewardClear
  ttySize=shutil.get_terminal_size()
  rerender()

fd = sys.stdin.fileno()
old_settings = termios.tcgetattr(fd)
try:
  tty.setraw(fd)
  writeToStdout("\x1b[?1049h") # use alternative screen buffer
  writeToStdout("\x1b[?25l") # hide cursor
  showInfo()
  signal.signal(signal.SIGWINCH,resChange) # everytime the terminal changes resolution, it sends the SIGWINCH signal
  grid.spawnTetrimino()
  renderHoldPiece()
  while True:
    char = None
    if select.select([sys.stdin], [], [], 0) == ([sys.stdin], [], []):
      char = sys.stdin.read(1)
      if char=="\x1b":
        if sys.stdin.read(1)=="[":
          ansiChar=sys.stdin.read(1)
          match ansiChar:
            case "A": char="up"
            case "B": char="down"
            case "C": char="right"
            case "D": char="left"
      if char=="\x03" or char=="q": break
    grid.update(
      int(char=="left" or char=="4")+int(char=="s")*columns,
      int(char=="right" or char=="6")+int(char=="d")*columns,
      char=="down" or char=="2",
      char=="up" or char=="x" or char=="1" or char=="5" or char=="9",
      char=="z" or char=="3" or char=="7",
      char=="a",
      char=="," or char==".",
      char==" " or char=="8",
      char=="c" or char=="0"
    )
    render()
    time.sleep(1/60)
    if grid.gameOver:
      writeToStdout(f"\x1b[{ttySize.lines};1H\x1b[0;7m\x1b[2K")
      gameOverText="game over - press enter to exit, or press r to restart"
      writeToStdout(gameOverText+" "*(ttySize.columns-len(gameOverText)))
      sys.stdout.flush()
      while not char in ["\n","\r","\x03","r"]:
        char = sys.stdin.read(1)
        continue
      if char!="r":
        break
      else:
        writeToStdout("\x1b[0m")
        showInfo()
    if char=="p":
      writeToStdout(f"\x1b[{ttySize.lines};1H\x1b[0;7m\x1b[2K")
      pauseText="game paused"
      writeToStdout(pauseText+" "*(ttySize.columns-len(pauseText)))
      sys.stdout.flush()
      while sys.stdin.read(1)!="p":
        continue
      writeToStdout("\x1b[0m")
      showInfo()
    if char=="r":
      grid=TetrisGrid(columns,rows+invisibleRows,spawnPiecesRotated,eventHandler)
      grid.spawnTetrimino()
      renderHoldPiece()
finally:
  writeToStdout("\x1b[?25h") # show cursor
  writeToStdout("\x1b[0m") # remove all attributes
  writeToStdout("\x1b[?1049l") # stop using alternative screen buffer
  termios.tcsetattr(fd, termios.TCSAFLUSH, old_settings)
