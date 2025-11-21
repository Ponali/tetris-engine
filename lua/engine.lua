engineVersion="1.0.0"

-- NOTE: y positions are reversed on these tables: "-1" means 1 down, and not 1 up.
standardWallKicksCW={
  {{ 0, 0},{-1, 0},{-1, 1},{ 0,-2},{-1,-2}},
  {{ 0, 0},{ 1, 0},{ 1,-1},{ 0, 2},{ 1, 2}},
  {{ 0, 0},{ 1, 0},{ 1, 1},{ 0,-2},{ 1,-2}},
  {{ 0, 0},{-1, 0},{-1,-1},{ 0, 2},{-1, 2}}
}
standardWallKicksCCW={
  {{ 0, 0},{ 1, 0},{ 1, 1},{ 0,-2},{ 1,-2}},
  {{ 0, 0},{ 1, 0},{ 1,-1},{ 0, 2},{ 1, 2}},
  {{ 0, 0},{-1, 0},{-1, 1},{ 0,-2},{-1,-2}},
  {{ 0, 0},{-1, 0},{-1,-1},{ 0, 2},{-1, 2}}
}
standardWallKicks180={ -- from tetr.io
  {{ 0, 0},{ 0, 1},{ 1, 1},{-1, 1},{ 1, 0},{-1, 0}},
  {{ 0, 0},{ 1, 0},{ 1, 2},{ 1, 1},{ 0, 2},{ 0, 1}},
  {{ 0, 0},{ 0,-1},{-1,-1},{ 1,-1},{-1, 0},{ 1, 0}},
  {{ 0, 0},{-1, 0},{-1, 2},{-1, 1},{ 0, 2},{ 0, 1}}
}
longPieceWallKicksCW={
  {{ 0, 0},{-2, 0},{ 1, 0},{-2,-1},{ 1, 2}},
  {{ 0, 0},{-1, 0},{ 2, 0},{-1, 2},{ 2,-1}},
  {{ 0, 0},{ 2, 0},{-1, 0},{ 2, 1},{-1,-2}},
  {{ 0, 0},{ 1, 0},{-2, 0},{ 1,-2},{-2, 1}}
}
longPieceWallKicksCCW={
  {{ 0, 0},{-1, 0},{ 2, 0},{-1, 2},{ 2,-1}},
  {{ 0, 0},{ 2, 0},{-1, 0},{ 2, 1},{-1,-2}},
  {{ 0, 0},{ 1, 0},{-2, 0},{ 1,-2},{-2, 1}},
  {{ 0, 0},{-2, 0},{ 1, 0},{-2,-1},{ 1, 2}}
}
longPieceWallKicks180={ -- i couldn't find anything on 180° rotations for I pieces, so this will be left unimplemented
  {{ 0, 0}},
  {{ 0, 0}},
  {{ 0, 0}},
  {{ 0, 0}}
}

tetrominoData={{
  {{0,0,0,0},
   {1,1,1,1},
   {0,0,0,0},
   {0,0,0,0}},
  {{0,0,1,0},
   {0,0,1,0},
   {0,0,1,0},
   {0,0,1,0}},
  {{0,0,0,0},
   {0,0,0,0},
   {1,1,1,1},
   {0,0,0,0}},
  {{0,1,0,0},
   {0,1,0,0},
   {0,1,0,0},
   {0,1,0,0}}
},{
  {{0,1,1,0},
   {0,1,1,0},
   {0,0,0,0},
   {0,0,0,0}},
  {{0,1,1,0},
   {0,1,1,0},
   {0,0,0,0},
   {0,0,0,0}},
  {{0,1,1,0},
   {0,1,1,0},
   {0,0,0,0},
   {0,0,0,0}},
  {{0,1,1,0},
   {0,1,1,0},
   {0,0,0,0},
   {0,0,0,0}}
},{
  {{0,1,0,0},
   {1,1,1,0},
   {0,0,0,0},
   {0,0,0,0}},
  {{0,1,0,0},
   {0,1,1,0},
   {0,1,0,0},
   {0,0,0,0}},
  {{0,0,0,0},
   {1,1,1,0},
   {0,1,0,0},
   {0,0,0,0}},
  {{0,1,0,0},
   {1,1,0,0},
   {0,1,0,0},
   {0,0,0,0}}
},{
  {{0,0,1,0},
   {1,1,1,0},
   {0,0,0,0},
   {0,0,0,0}},
  {{0,1,0,0},
   {0,1,0,0},
   {0,1,1,0},
   {0,0,0,0}},
  {{0,0,0,0},
   {1,1,1,0},
   {1,0,0,0},
   {0,0,0,0}},
  {{1,1,0,0},
   {0,1,0,0},
   {0,1,0,0},
   {0,0,0,0}}
},{
  {{1,0,0,0},
   {1,1,1,0},
   {0,0,0,0},
   {0,0,0,0}},
  {{0,1,1,0},
   {0,1,0,0},
   {0,1,0,0},
   {0,0,0,0}},
  {{0,0,0,0},
   {1,1,1,0},
   {0,0,1,0},
   {0,0,0,0}},
  {{0,1,0,0},
   {0,1,0,0},
   {1,1,0,0},
   {0,0,0,0}}
},{
  {{0,1,1,0},
   {1,1,0,0},
   {0,0,0,0},
   {0,0,0,0}},
  {{0,1,0,0},
   {0,1,1,0},
   {0,0,1,0},
   {0,0,0,0}},
  {{0,0,0,0},
   {0,1,1,0},
   {1,1,0,0},
   {0,0,0,0}},
  {{1,0,0,0},
   {1,1,0,0},
   {0,1,0,0},
   {0,0,0,0}}
},{
  {{1,1,0,0},
   {0,1,1,0},
   {0,0,0,0},
   {0,0,0,0}},
  {{0,0,1,0},
   {0,1,1,0},
   {0,1,0,0},
   {0,0,0,0}},
  {{0,0,0,0},
   {1,1,0,0},
   {0,1,1,0},
   {0,0,0,0}},
  {{0,1,0,0},
   {1,1,0,0},
   {1,0,0,0},
   {0,0,0,0}}
}}

-- usually this appears in OpenComputers, but it doesn't in actual lua, so i have to copy it from ocelot's machine.lua
if not _G.checkArg then
  function _G.checkArg(n, have, ...)
    have = type(have)
    local function check(want, ...)
      if not want then
        return false
      else
        return have == want or check(...)
      end
    end
    if not check(...) then
      local msg = string.format("bad argument #%d (%s expected, got %s)",
                                n, table.concat({...}, " or "), have)
      error(msg, 3)
    end
  end
end

function Grid2D(cols,rows)
  local object = {}
  function object.get(s,x,y,replace)
    checkArg(1,s,"table")
    checkArg(2,x,"number")
    checkArg(3,y,"number")
    if x<1 or y<1 or x>s.cols or y>s.rows then
      return replace -- if not specified, it will return nil anyway
    end
    return s.arr[x+(y-1)*s.cols]
  end
  function object.set(s,x,y,v)
    checkArg(1,s,"table")
    checkArg(2,x,"number")
    checkArg(3,y,"number")
    checkArg(4,v,"number")
    if x<1 or y<1 or x>s.cols or y>s.rows then
      return false
    end
    s.arr[x+(y-1)*s.cols]=v
    return true
  end
  function object.copy(s)
    checkArg(1,s,"table")
    local n=Grid2D(s.cols,s.rows)
    n.arr={table.unpack(s.arr)} -- copy table
    return n
  end
  object.arr={}
  for _=1,cols*rows do
    table.insert(object.arr,0)
  end
  object.cols=cols
  object.rows=rows
  return object
end

-- Source - https://stackoverflow.com/a
-- Posted by MHebes, modified by community. See post 'Timeline' for change history
-- Retrieved 2025-11-07, License - CC BY-SA 4.0

local function ShuffleInPlace(t)
  for i = #t, 2, -1 do
    local j = math.random(i)
    t[i], t[j] = t[j], t[i]
  end
end

function TetrisBag()
  local s={}
  function s.addRandomPieces(s)
    checkArg(1,s,"table")
    local pieceList={}
    for i=1,7 do
      table.insert(pieceList,i-1+math.random(0,1)*7)
    end
    ShuffleInPlace(pieceList)
    for i=1,#pieceList do
      table.insert(s.outgoing,pieceList[i])
    end
  end
  function s.pop(s)
    checkArg(1,s,"table")
    local piece = table.remove(s.outgoing,1)
    if #s.outgoing==7 then
      s:addRandomPieces()
    end
    return piece
  end
  s.outgoing={}
  s:addRandomPieces()
  s:addRandomPieces()
  return s
end

function TetrisGrid(cols,rows,spawnHeight,eventHandler)
  checkArg(1,cols,"number")
  checkArg(2,rows,"number")
  checkArg(3,spawnHeight,"number")
  checkArg(4,eventHandler,"function")
  local s={}
  function s.getGravitySpeed(s)
    checkArg(1,s,"table")
    return (800-((s.level-1)*7))
    -- formula from Tetris Worlds: https://harddrop.com/wiki/Tetris_Worlds -> Gravity
  end
  function s.getLockDelayTime(s)
    checkArg(1,s,"table")
    return 500 -- the lock delay doesn't change, and is always set to be half a second
  end
  function s.tetriminoColliding(s,x,y,t,r,ground)
    checkArg(1,s,"table")
    checkArg(2,x,"number")
    checkArg(3,y,"number")
    checkArg(4,t,"number")
    checkArg(5,r,"number")
    checkArg(6,ground,"nil","boolean")
    local d=tetrominoData[t+1][r+1]
    for i=1,4 do
      for j=1,4 do
        if d[i][j]==1 then
          local vx=j+x
          local vy=i+y
          if vx<=0 or vy<=0 or vx>s.cols or vy>s.rows then
            if ground then
              return vy>s.rows
            else
              return true
            end
          elseif s.grid:get(vx,vy)>0 then
            return true
          end
        end
      end
    end
    return false
  end
  function s.spawnSpecificTetrimino(s,tt)
    checkArg(1,s,"table")
    checkArg(2,tt,"number")
    s.tx=(s.cols-4)//2
    s.ty=s.spawnHeight
    s.tt=tt
    s.tr=0
    if s.tt>=7 then
      s.tt=s.tt-7
      -- make piece spawn 180° with s.ty 1 less
      if s.spawnPiecesRotated then
        s.ty=3
        s.tr=2
      end
    end
  end
  function s.spawnTetrimino(s)
    checkArg(1,s,"table")
    s:spawnSpecificTetrimino(s.bag:pop())
    s.pieceLock = false
    s.pieceLockTimestamp=0
    s.pieceTSpin = false
    if s:tetriminoColliding(s.tx,s.ty,s.tt,s.tr) then
      s.gameOver = true
    end
    s.pieceLockResetMoves=15
    s:checkPieceLock()
    if not s.canHoldPiece then
      s.canHoldPiece = true
      s:eventHandler("updateHoldPiece",{})
    end
    s:eventHandler("updateNext",{})
  end
  function s.checkPieceLock(s)
    checkArg(1,s,"table")
    if s:tetriminoColliding(s.tx,s.ty+1,s.tt,s.tr,true) then
      if not s.pieceLock then
        s.pieceLock = true
        s.pieceLockTimestamp = getTimestamp()
      else
        s.pieceLockResetMoves=s.pieceLockResetMoves-1
        if s.pieceLockResetMoves>0 then
          s.pieceLockTimestamp=getTimestamp()
        end
      end
    else
      s.pieceLock = false
    end
  end
  function s.checkTSpin(s)
    checkArg(1,s,"table")
    if s.tt~=2 then return end
    if s.grid:get(s.tx+1,s.ty+1,1)>0 or s.grid:get(s.tx+3,s.ty+1,1)>0 then -- top part
      if s.grid:get(s.tx+1,s.ty+3,1)>0 and s.grid:get(s.tx+3,s.ty+3,1)>0 then -- bottom part
        s.pieceTSpin = true
      end
    end
  end
  function s.isMiniTSpin(s)
    checkArg(1,s,"table")
    if s.tt~=2 then return false end
    if s.tr==0 then return not (s.grid:get(s.tx+1,s.ty+1)>0 and s.grid:get(s.tx+3,s.ty+1)>0) end
    if s.tr==1 then return not (s.grid:get(s.tx+3,s.ty+1)>0 and s.grid:get(s.tx+3,s.ty+3)>0) end
    if s.tr==2 then return not (s.grid:get(s.tx+3,s.ty+3)>0 and s.grid:get(s.tx+1,s.ty+3)>0) end
    if s.tr==3 then return not (s.grid:get(s.tx+1,s.ty+3)>0 and s.grid:get(s.tx+1,s.ty+1)>0) end
  end
  function s.holdCurrentPiece(s)
    checkArg(1,s,"table")
    if not s.canHoldPiece then return end
    local heldPiece = s.holdPiece
    s.holdPiece=s.tt
    if type(heldPiece)=="number" then
      s:spawnSpecificTetrimino(heldPiece)
    else
      s:spawnTetrimino()
    end
    s.canHoldPiece = false
    s:eventHandler("updateHoldPiece",{})
  end
  function s.movePiece(s,vx,vy)
    checkArg(1,s,"table")
    checkArg(2,vx,"number")
    checkArg(3,vy,"number")
    if not s:tetriminoColliding(s.tx+vx,s.ty+vy,s.tt,s.tr) then
      s.tx=s.tx+vx
      s.ty=s.ty+vy
      s:checkPieceLock()
      return true
    end
    return false
  end
  function s.rotatePieceCW(s)
    checkArg(1,s,"table")
    local newr=(s.tr+1)%4
    local kickData = standardWallKicksCW[s.tr+1]
    if s.tt==0 then
      kickData = longPieceWallKicksCW[s.tr+1]
    end
    for i=1,#kickData do
      if not s:tetriminoColliding(s.tx+kickData[i][1],s.ty-kickData[i][2],s.tt,newr) then
        s.tx=s.tx+kickData[i][1]
        s.ty=s.ty-kickData[i][2]
        s.tr=newr
        s:checkPieceLock()
        s:checkTSpin()
        return
      end
    end
  end
  function s.rotatePieceCCW(s)
    checkArg(1,s,"table")
    local newr=(s.tr+3)%4
    local kickData = standardWallKicksCCW[s.tr+1]
    if s.tt==0 then
      kickData = longPieceWallKicksCCW[s.tr+1]
    end
    for i=1,#kickData do
      if not s:tetriminoColliding(s.tx+kickData[i][1],s.ty-kickData[i][2],s.tt,newr) then
        s.tx=s.tx+kickData[i][1]
        s.ty=s.ty-kickData[i][2]
        s.tr=newr
        s:checkPieceLock()
        s:checkTSpin()
        return
      end
    end
  end
  function s.rotatePiece180(s)
    checkArg(1,s,"table")
    local newr=(s.tr+2)%4
    local kickData = standardWallKicks180[s.tr+1]
    if s.tt==0 then
      kickData = longPieceWallKicks180[s.tr+1]
    end
    for i=1,#kickData do
      if not s:tetriminoColliding(s.tx+kickData[i][1],s.ty-kickData[i][2],s.tt,newr) then
        s.tx=s.tx+kickData[i][1]
        s.ty=s.ty-kickData[i][2]
        s.tr=newr
        s:checkPieceLock()
        s:checkTSpin()
        return
      end
    end
  end
  function s.isAllClear(s)
    checkArg(1,s,"table")
    for i=1,s.rows do
      for j=1,s.cols do
        if s.grid:get(j,i)>0 then
          return false
        end
      end
    end
    return true
  end
  function s.scoreClearedLines(s,linesFound,isMiniTSpin)
    checkArg(1,s,"table")
    checkArg(2,linesFound,"number")
    checkArg(3,isMiniTSpin,"boolean")
    if linesFound>0 then
      s.combo=s.combo+1
    else
      s.combo=0
    end
    if s.combo>=2 then
      s.score=s.score+50*(s.combo-1)*s.level
    end
    -- handle back-to-back
    if linesFound>0 then
      if linesFound<4 and not s.pieceTSpin then
        s.backToBack=0
      else
        s.backToBack=s.backToBack+1
      end
    end
    local b2bMultiplier=1
    if s.backToBack>2 then
      b2bMultiplier=1.5
    end
    -- handle "all clear"
    local allClear=s:isAllClear()
    -- add points to score
    if allClear then
      if s.backToBack>2 and linesFound>=4 then
        s.score=s.score+3200*s.level
      else
        if linesFound==1 then s.score=s.score+800*s.level*b2bMultiplier end
        if linesFound==2 then s.score=s.score+1200*s.level*b2bMultiplier end
        if linesFound==3 then s.score=s.score+1800*s.level*b2bMultiplier end
        if linesFound==4 then s.score=s.score+2000*s.level*b2bMultiplier end
      end
    else
      if s.pieceTSpin then
        if isMiniTSpin then
          if linesFound==0 then s.score=s.score+100*s.level end
          if linesFound==1 then s.score=s.score+200*s.level*b2bMultiplier end
          if linesFound==2 then s.score=s.score+400*s.level*b2bMultiplier end
        else
          if linesFound==0 then s.score=s.score+400*s.level end
          if linesFound==1 then s.score=s.score+800*s.level*b2bMultiplier end
          if linesFound==2 then s.score=s.score+1200*s.level*b2bMultiplier end
          if linesFound==3 then s.score=s.score+1600*s.level*b2bMultiplier end
        end
      else
        if linesFound==1 then s.score=s.score+100*s.level*b2bMultiplier end
        if linesFound==2 then s.score=s.score+300*s.level*b2bMultiplier end
        if linesFound==3 then s.score=s.score+500*s.level*b2bMultiplier end
        if linesFound==4 then s.score=s.score+800*s.level*b2bMultiplier end
      end
    end
    -- update level
    while s.levelLines>10 do
      s.level=s.level+1
      s.levelLines=s.levelLines-10
    end
    -- send t-spin reward
    if s.pieceTSpin then
      s:eventHandler("tSpin",{lines=linesFound,mini=isMiniTSpin})
    elseif linesFound>0 then
      s:eventHandler("lineClear",{lines=linesFound,allClear=allClear})
    end
    s:eventHandler("updateScore")
  end
  function s.clearLines(s)
    checkArg(1,s,"table")
    local isMiniTSpin = s:isMiniTSpin()
    local linesFound=0
    local function removeLine(y)
      -- add to score
      s.linesCleared=s.linesCleared+1
      s.levelLines=s.levelLines+1
      linesFound=linesFound+1
      -- copy
      for i=y-1,1,-1 do
        for j=1,s.cols do
          s.grid:set(j,i+1,s.grid:get(j,i))
        end
      end
      -- remove top line
      for j=1,s.cols do
        s.grid:set(j,0,0)
      end
    end
    local function clearCheck(y)
      local lineClear = true
      for x=1,s.cols do
        if s.grid:get(x,y)==0 then
          lineClear = false
        end
      end
      return lineClear
    end
    local linesToClear={}
    for y=1,s.rows do
      if clearCheck(y) then
        table.insert(linesToClear,y)
      end
    end
    if #linesToClear>0 then
      s:eventHandler("lineClearAnim",{lines=linesToClear})
    end
    while #linesToClear>0 do
      local y=table.remove(linesToClear,1)
      removeLine(y)
    end
    s:scoreClearedLines(linesFound,isMiniTSpin)
  end
  function s.placeCurrentPiece(s)
    checkArg(1,s,"table")
    s:eventHandler("place",{})
    local d=tetrominoData[s.tt+1][s.tr+1]
    for i=1,4 do
      for j=1,4 do
        if d[i][j]==1 then
          s.grid:set(j+s.tx,i+s.ty,s.tt+1)
        end
      end
    end
    s:clearLines()
    s:spawnTetrimino()
  end
  function s.dropCurrentPiece(s,place)
    checkArg(1,s,"table")
    checkArg(2,place,"boolean")
    local amount=0
    while s:movePiece(0,1) do
      amount=amount+1
    end
    if place then
      s:placeCurrentPiece()
    end
    if place then
      s.score=s.score+2*amount
    else
      s.score=s.score+1*amount
    end
    s:eventHandler("updateScore",{})
  end
  function s.getHardDropYPosition(s)
    checkArg(1,s,"table")
    for i=s.ty,s.rows-1 do
      if s:tetriminoColliding(s.tx,i,s.tt,s.tr) then
        return i-1
      end
    end
  end
  function s.getScreenWithCurrentPiece(s)
    checkArg(1,s,"table")
    local scr=s.grid:copy()
    local d=tetrominoData[s.tt+1][s.tr+1]
    local shadowY=s:getHardDropYPosition()
    for i=1,4 do
      for j=1,4 do
        if d[i][j]==1 then
          scr:set(j+s.tx,i+shadowY,8) -- shadow
          scr:set(j+s.tx,i+s.ty,s.tt+1)
        end
      end
    end
    return scr
  end
  function s.cellOnCurrentPiece(s,x,y)
    checkArg(1,s,"table")
    checkArg(2,x,"number")
    checkArg(3,y,"number")
    local d=tetrominoData[s.tt+1][s.tr+1]
    local vx=x-s.tx-1
    local vy=y-s.ty-1
    if vx<0 or vy<0 or vx>=4 or vy>=4 then
      return false
    end
    return d[vy+1][vx+1]==1
  end
  function s.pause(s,pauseValue)
    checkArg(1,s,"table")
    checkArg(2,pauseValue,"nil","boolean")
    if pauseValue==nil then
      pauseValue=not s.paused
    end
    if s.paused~=pauseValue then
      s.paused=pauseValue
      if pauseValue then
        s.pauseTimestamp=getTimestamp()
      else
        local pauseTime=getTimestamp()-s.pauseTimestamp
        s.gravityUpdate=s.gravityUpdate+pauseTime
        s.pieceLockTimestamp=s.pieceLockTimestamp+pauseTime
        -- NOTE: if any more timestamps show up, make sure to put them here
      end
    end
  end
  function s.update(s,left,right,down,rotateCW,rotateCCW,rotate180,sonicDrop,hardDrop,holdPiece)
    checkArg(1,s,"table")
    checkArg(2,left,"number","boolean")
    checkArg(3,right,"number","boolean")
    checkArg(4,down,"number","boolean")
    checkArg(5,rotateCW,"boolean")
    checkArg(6,rotateCCW,"boolean")
    checkArg(7,rotate180,"boolean")
    checkArg(8,sonicDrop,"boolean")
    checkArg(9,hardDrop,"boolean")
    checkArg(10,holdPiece,"boolean")
    if s.gameOver then return end
    if type(left)=="boolean" then
      left = left and 1 or 0
    end
    if type(right)=="boolean" then
      right = right and 1 or 0
    end
    if type(down)=="boolean" then
      down = down and 1 or 0
    end
    if sonicDrop then     s:dropCurrentPiece(false)
    elseif hardDrop then  s:dropCurrentPiece(true)
    elseif left>0 then    for i=1,left  do s:movePiece(-1, 0) end
    elseif right>0 then   for i=1,right do s:movePiece( 1, 0) end
    elseif down>0 then    for i=1,down  do s:movePiece( 0, 1) end
                          s.score=s.score+1*down
                          s:eventHandler("updateScore",{})
    elseif rotateCW then  s:rotatePieceCW()
    elseif rotateCCW then s:rotatePieceCCW()
    elseif rotate180 then s:rotatePiece180()
    elseif holdPiece then s:holdCurrentPiece() end
    if s.gravityUpdate+s:getGravitySpeed()<getTimestamp() then
      s.gravityUpdate=getTimestamp()
      s:movePiece( 0, 1)
    end
    if s.pieceLock and s.pieceLockTimestamp+s:getLockDelayTime()<getTimestamp() then
      s:placeCurrentPiece()
    end
  end
  s.grid=Grid2D(cols,rows)
  s.bag=TetrisBag()
  s.gravityUpdate=getTimestamp()
  s.pieceLock = false
  s.pieceLockTimestamp = 0
  s.pieceLockResetMoves = 15
  s.pieceTSpin = false
  s.cols=cols
  s.rows=rows
  s.spawnPiecesRotated=false
  s.spawnHeight=spawnHeight
  s.eventHandler=eventHandler
  s.linesCleared = 0
  s.levelLines = 0
  s.level = 1
  s.score = 0
  s.combo = 0
  s.paused = false
  s.pauseTimestamp = 0
  s.backToBack = 0
  -- s.holdPiece = nil
  s.canHoldPiece = true
  s.gameOver = false
  return s
end

return {
  engineVersion=engineVersion,
  tetrominoData=tetrominoData,
  standardWallKicksCW=standardWallKicksCW,
  standardWallKicksCCW=standardWallKicksCCW,
  standardWallKicks180=standardWallKicks180,
  longPieceWallKicksCW=longPieceWallKicksCW,
  longPieceWallKicksCCW=longPieceWallKicksCCW,
  longPieceWallKicks180=longPieceWallKicks180,
  TetrisBag=TetrisBag,
  Grid2D=Grid2D,
  TetrisGrid=TetrisGrid,
}
