frontendVersion="1.0.0a"

local clockStart = os.clock()
function getTimestamp()
  return (os.clock()-clockStart)*1000
end

for i,v in pairs(require("engine")) do _G[i]=v end
local posix = require("posix")

if not os.execute("stty > /dev/null 2>&1") then
  return print("\x1b[31mThe stty command is not installed on your system, which is required to make the terminal use raw mode.\nPlease use another frontend, another terminal frontend in another language, or install the stty command.\x1b[0m")
end

if not os.execute("dd --version > /dev/null 2>&1") then
  return print("\x1b[31mThe dd command is not installed on your system, which is required to make the terminal read user keystrokes.\nPlease use another frontend, another terminal frontend in another language, or install the dd command.\x1b[0m")
end

local columns = 10
local rows = 20
local invisibleRows = 4
local spawnPiecesRotated = false
local elektronikaMode = false
local fastRenderMode = false

for i,v in pairs(arg) do
  if i==0 then goto continue end
  if string.find(v,"=") then
    local name=v:sub(1,string.find(v,"=")-1)
    local value=v:sub(string.find(v,"=")+1)
    if name=="w" or name=="width" or name=="c" or name=="cols" or name=="columns" then
      columns=tonumber(value)
    end
    if name=="h" or name=="height" or name=="r" or name=="rows" or name=="l" or name=="lines" then
      rows=tonumber(value)
    end
    if name=="ir" or name=="irows" or name=="bz" or name=="bufferzone" then
      invisibleRows=tonumber(value)
    end
  else
    if v=="sr" or v=="rotate" or v=="spawnrotated" then
      spawnPiecesRotated=true
    end
    if v=="alexey" or v=="leonidovich" or v=="pajitnov" or v=="elektronika" then
      elektronikaMode=true
    end
    if v=="fast" or v=="fastrender" or v=="boxrender" then
      fastRenderMode=true
    end
  end
  ::continue::
end

local rewardColor={255,255,255}
local rewardString=""
local rewardTimestamp=0
local rewardClear=false

local linesCleared={}
for i=1,rows do table.insert(linesCleared,false) end
local lineClearTimestamp=0


local writeToStdout = io.write
local ttySize = {}
local grid = {}

local function getGridPos()
  local gridX = (ttySize.columns-columns*2)//2
  local gridY = (ttySize.lines-rows)//2
  return gridX,gridY
end

local function doubleSize(str)
  return "\x1b#3"..str.."\n\x1b#4\r"..str
end

local function round(n)
  return math.floor(n+0.5)
end

local function center(str,res)
  return string.rep(" ",round((res-#str)/2))..str
end

local function getCellColor(cell)
  if elektronikaMode then
    return {0,255,0}
  end
  if cell==0 then return {  0,  0,  0} end
  if cell==1 then return {  0,255,255} end
  if cell==2 then return {255,255,  0} end
  if cell==3 then return {255,  0,255} end
  if cell==4 then return {255,127,  0} end
  if cell==5 then return {  0,  0,255} end
  if cell==6 then return {  0,255,  0} end
  if cell==7 then return {255,  0,  0} end
  if cell==8 then return { 80, 80, 80} end
end

local gridBefore = nil
local function renderGrid(usePieceDim)
  local gridX,gridY = getGridPos()
  local pieceDim = 1
  if grid.pieceLock then
    local timePassed = (getTimestamp()-grid.pieceLockTimestamp)/grid:getLockDelayTime()
    pieceDim = 1-timePassed/2
  end
  local scr=grid:getScreenWithCurrentPiece()
  if elektronikaMode then
    writeToStdout("\x1b[38;2;0;255;0m")
  end
  for y=1+invisibleRows,rows+invisibleRows do
    for x=1,columns do
      local cell=scr:get(x,y)
      if (gridBefore~=nil and gridBefore:get(x,y)==cell) and not (grid:cellOnCurrentPiece(x,y) or (linesCleared[y] and getTimestamp()-lineClearTimestamp<600)) then
        goto continue
      end
      writeToStdout("\x1b["..(gridY+(y-1)-invisibleRows)..";"..(gridX+(x-1)*2).."f")
      if elektronikaMode then
        if cell==0 or cell==8 then
          writeToStdout(" .")
        else
          writeToStdout("[]")
        end
      else
        local r,g,b = table.unpack(getCellColor(cell))
        if grid:cellOnCurrentPiece(x,y) and usePieceDim then
          r=r*pieceDim
          g=g*pieceDim
          b=b*pieceDim
        end
        if linesCleared[y] then
          local light=math.max(math.min(1.2-(getTimestamp()-lineClearTimestamp)/500,1),0)
          r=math.max(math.min(r+light*70,255),0)
          g=math.max(math.min(g+light*70,255),0)
          b=math.max(math.min(b+light*70,255),0)
        end
        local dr=math.max(math.min(r-15,255),0)
        local dg=math.max(math.min(g-15,255),0)
        local db=math.max(math.min(b-15,255),0)
        local br=math.max(math.min(r+60,255),0)
        local bg=math.max(math.min(g+60,255),0)
        local bb=math.max(math.min(b+60,255),0)
        if cell==0 or cell==8 or fastRenderMode then
          writeToStdout("\x1b[48;2;"..math.floor(r)..";"..math.floor(g)..";"..math.floor(b).."m  ")
        else
          writeToStdout("\x1b[48;2;"..math.floor(r)..";"..math.floor(g)..";"..math.floor(b).."m\x1b[38;2;"..math.floor(dr)..";"..math.floor(dg)..";"..math.floor(db).."m🬿\x1b[38;2;"..math.floor(br)..";"..math.floor(bg)..";"..math.floor(bb).."m🭥")
        end
      end
      ::continue::
    end
  end
  gridBefore=scr
end

local function renderPiece(piece,col)
  local rotation=0
  if piece>=7 then
    piece=piece-7
    if spawnPiecesRotated then
      rotation=2
    end
  end
  local d=tetrominoData[piece+1][rotation+1]
  local r,g,b = table.unpack(getCellColor(piece+1))
  if col~=nil then
    writeToStdout("\x1b[38;2;"..col[1]..";"..col[2]..";"..col[3].."m")
  else
    writeToStdout("\x1b[38;2;"..r..";"..g..";"..b.."m")
  end
  local blocks = {[0]=" ","▀","▄","█"}
  for i=1,4,2 do
    for j=1,4 do
      writeToStdout(blocks[d[i][j]+d[i+1][j]*2])
    end
    writeToStdout("\x1b[4D\n")
  end
end

local function showInfo()
  writeToStdout("\x1b["..ttySize.lines..";1f\x1b#5\x1b[2m\x1b[2K")
  local text="tetris-engine v"..engineVersion.." terminal frontend v"..frontendVersion.." (lua)"
  writeToStdout(text..string.rep(" ",ttySize.columns-#text))
  writeToStdout("\x1b[0m")
end

local function renderReward()
  if elektronikaMode then
    rewardColor={0,255,0}
  end
  local posY = math.min((ttySize.lines+rows+2)//2,ttySize.lines-2)
  writeToStdout("\x1b["..posY..";1f")
  local fade = math.max(math.min(2.5-(getTimestamp()-rewardTimestamp)/600,1),0)
  local r=fade*rewardColor[1]
  local g=fade*rewardColor[2]
  local b=fade*rewardColor[3]
  writeToStdout("\x1b[49m\x1b[38;2;"..math.floor(r)..";"..math.floor(g)..";"..math.floor(b).."m")
  if fade>0 then
    local ansiDim = ""
    if fade<0.5 then
      ansiDim="\x1b[2m"
    end
    writeToStdout(doubleSize("\x1b[2K"..ansiDim..center(rewardString,ttySize.columns//2)))
  else
    writeToStdout(doubleSize("\x1b[2K"))
    rewardClear=false
  end
end
local function renderSide()
  local gridX,gridY=getGridPos()
  writeToStdout("\x1b[0m\x1b["..gridY..";"..(gridX+columns*2+1).."f")
  for i=1,4 do
    renderPiece(grid.bag.outgoing[i])
  end
  if elektronikaMode then
    writeToStdout("\x1b[38;2;0;255;0m")
  else
    writeToStdout("\x1b[39;49m")
  end
  writeToStdout("\x1b[s")
  local clearLine="\x1b[K"
  local str=string.format("level %d\nlines: %d\nscore: %06d",grid.level,grid.linesCleared,grid.score)
  if grid.combo>=2 then
    str=str.."\n\x1b[0m\x1b[1mcombo x"..grid.combo.."\x1b[22m"
    -- turns out, using "0m" instead of "39m" is required to have the plasma-themed bold color
  end
  if grid.backToBack>=2 then
    str=str.."\n\x1b[0m\x1b[1mback-to-back x"..grid.backToBack.."\x1b[22m"
  end
  str=str.."\n\n"
  writeToStdout(clearLine..string.gsub(str,"\n","\x1b[u\n\x1b[s"..clearLine))
  io.flush()
end
local function renderHoldPiece()
  local gridX,gridY=getGridPos()
  writeToStdout("\x1b[39m\x1b[48;2;50;50;50m")
  if elektronikaMode then
    writeToStdout("\x1b[49m\x1b[38;2;0;255;0m")
  end
  writeToStdout("\x1b["..gridY..";"..(gridX-6).."f")
  writeToStdout(string.rep(string.rep(" ",6).."\n\x1b[6D",3).."\x1b[3A")
  writeToStdout(" HOLD\n\x1b[4D")
  if grid.holdPiece==nil then
    writeToStdout("\x1b[38;2;100;100;100mnone")
  else
    local col=nil
    if not grid.canHoldPiece then
      col={120,120,120}
    end
    renderPiece(grid.holdPiece,col)
  end
  writeToStdout("\x1b[39;49m")
  io.flush()
end
local function render()
  local gridX,gridY=getGridPos()
  renderGrid(true)
  if rewardClear then
    renderReward()
  end
  io.flush()
end
local function rerender()
  writeToStdout("\x1b[0m\x1b[H\x1b[2J") -- clear screen
  showInfo()
  gridBefore=nil
  renderGrid(true)
  rewardClear=true
  renderReward()
  renderSide()
  renderHoldPiece()
end

local function readChar()
  local handle = io.popen("dd bs=1 count=1 iflag=nonblock 2>/dev/null")
  local char = handle:read(1)
  handle:close()
  return char
end

local function getTermSize()
  writeToStdout("\x1b[s\x1b[999;999f\x1b[6n\x1b[u")
  local out=""
  while out:sub(-1,-1)~="R" do
    local char=readChar()
    if char~=nil then
      out=out..char
    end
  end
  out=out:sub(3,-2)
  local sep=string.find(out,";")
  local rows=tonumber(out:sub(1,sep-1))
  local cols=tonumber(out:sub(sep+1))
  return {
    columns=cols,
    lines=rows
  }
end

local function resChange()
  ttySize=getTermSize()
  rerender()
end

local function eventHandler(_,eventType,data)
  if eventType=="place" then renderGrid(false) end
  if eventType=="updateNext" or eventType=="updateScore" then renderSide() end
  if eventType=="updateHoldPiece" then renderHoldPiece() end
  if eventType=="tSpin" then
    rewardString=({[0]="t-spin","t-spin single","t-spin double","t-spin triple"})[data.lines]
    rewardColor={255,80,255}
    if data.mini then
      rewardString="mini "..rewardString
      rewardColor={210,170,210}
    end
    rewardTimestamp=getTimestamp()
    rewardClear=true
  end
  if eventType=="lineClearAnim" then
    linesCleared={}
    for i=1,rows do table.insert(linesCleared,false) end
    for _,y in pairs(data.lines) do
      linesCleared[y]=true
    end
    lineClearTimestamp=getTimestamp()
    gridBefore=nil
  end
  if eventType=="lineClear" then
    if data.allClear then
      rewardString="ALL CLEAR!"
      rewardColor={255,255,190}
    else
      if data.lines==1 then return end
      if data.lines==2 then
        rewardString="double"
        rewardColor={80,80,80}
      end
      if data.lines==3 then
        rewardString="triple"
        rewardColor={150,180,180}
      end
      if data.lines==4 then
        rewardString="tetris"
        rewardColor={80,255,255}
      end
    end
    rewardTimestamp=getTimestamp()
    rewardClear=true
  end
end

grid = TetrisGrid(columns,rows+invisibleRows,spawnPiecesRotated,eventHandler)

-- these 2 functions from "plterm": https://github.com/philanc/plterm/blob/master/plterm.lua#L386

-- the string meaning that file:read() should return all the
-- content of the file is "*a"  for Lua 5.0-5.2 and LuaJIT,
-- and "a" for more recent Lua versions
-- thanks to Phil Hagelberg for the heads up.
local READALL = (_VERSION < "Lua 5.3") and "*a" or "a"
local function getTTYMode()
  local fh = io.popen("stty -g")
  local mode = fh:read(READALL)
  local succ, e, msg = fh:close()
  return succ and mode or nil, e, msg
end

function setTTYMode(mode)
  return os.execute("stty " .. mode)
end

local function sleep(t)
  local endTime=getTimestamp()+t
  while getTimestamp()<endTime do end
end

local oldSettings = getTTYMode()
local status,err = pcall(function()
  os.execute("stty raw -echo 2> /dev/null")
  ttySize = getTermSize() -- this is done later because it needs raw mode stdin
  writeToStdout("\x1b[?1049h") -- use alternative screen buffer
  writeToStdout("\x1b[?25l") -- hide cursor
  showInfo()
  posix.signal.signal(posix.signal.SIGWINCH,resChange) -- everytime the terminal changes resolution, it sends the SIGWINCH signal
  grid:spawnTetrimino()
  renderHoldPiece()
  while true do
    local char=readChar()
    if char=="\x1b" then
      if readChar()=="[" then
        local ansiChar = readChar()
        if ansiChar=="A" then char="up" end
        if ansiChar=="B" then char="down" end
        if ansiChar=="C" then char="right" end
        if ansiChar=="D" then char="left" end
      end
    end
    if char=="\x03" or char=="q" then break end
    grid:update(
      ((char=="left" or char=="4") and 1 or 0)+(char=="s" and columns or 0),
      ((char=="right" or char=="6") and 1 or 0)+(char=="d" and columns or 0),
      char=="down" or char=="2",
      char=="up" or char=="x" or char=="1" or char=="5" or char=="9",
      char=="z" or char=="3" or char=="7",
      char=="a",
      char=="," or char==".",
      char==" " or char=="8",
      char=="c" or char=="0"
    )
    render()
    sleep(1/60)
    if grid.gameOver then
      writeToStdout("\x1b["..ttySize.lines..";1H\x1b[0;7m\x1b[2K")
      local gameOverText="game over - press enter to exit, or press r to restart"
      writeToStdout(gameOverText..string.rep(" ",ttySize.columns-#gameOverText))
      io.flush()
      while not (char=="\n" or char=="\r" or char=="\x03" or char=="r") do
        char = readChar()
      end
      if char~="r" then
        break
      else
        writeToStdout("\x1b[0m")
        showInfo()
      end
    end
    if char=="p" then
      writeToStdout("\x1b["..ttySize.lines..";1H\x1b[0;7m\x1b[2K")
      local pauseText="game paused"
      writeToStdout(pauseText..string.rep(" ",ttySize.columns-#pauseText))
      io.flush()
      while readChar()~="p" do end
      writeToStdout("\x1b[0m")
      showInfo()
    end
    if char=="r" then
      grid = TetrisGrid(columns,rows+invisibleRows,spawnPiecesRotated,eventHandler)
      grid:spawnTetrimino()
      renderHoldPiece()
    end
  end
end)
writeToStdout("\x1b[?25h") -- show cursor
writeToStdout("\x1b[0m") -- remove all attributes
writeToStdout("\x1b[?1049l") -- stop using alternative screen buffer
setTTYMode(oldSettings)
if not status then
  print("\x1b[31m"..err.."\x1b[0m")
end
