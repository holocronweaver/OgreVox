-- Randomly generating a random generator
require 'tablesaveload'
require 'noisemoduletree'

childrenletters={"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"}
-- Set of generators
leafGenerators=
{
	-- Fractal generator
	function(level,name,tbl)
		-- First, insert an autocorrect
		table.insert(tbl,1,
		{
			name=name,
			type="autocorrect",
			source=name.."Fractal",
			low=-1,
			high=1
		})
		
		-- Insert fractal
		table.insert(tbl,1,
		{
			name=name.."Fractal",
			type="fractal",
			fractaltype=randRange(anl.FBM, anl.HYBRIDMULTI),
			basistype=randRange(anl.VALUE, anl.SIMPLEX),
			interptype=randRange(anl.NONE, anl.QUINTIC),
			octaves=randRange(1,10),
			frequency=randRange(1,10)
		})
	end,
	
}
nodeGenerators=
{
	-- Bias
	function(level,name,tbl)
	
		-- First, decide if bias should come from constant or source
		if randRange(1,10)<=5 then
			table.insert(tbl,1,
			{
				name=name,
				type="bias",
				source=name..'A',
				bias=rand01()
			})
			generateNode(level-1,name..'A',tbl)
		else
			table.insert(tbl,1,
			{
				name=name,
				type="bias",
				source=name..'A',
				bias=name..'B'
			})
			generateNode(level-1,name..'A',tbl)
			generateNode(level-1,name..'B',tbl)
		end
	end,
	
	-- Blend
	function(level,name,tbl)
		table.insert(tbl,1,
		{
			name=name,
			type="blend",
			low=name..'A',
			high=name..'B',
			control=name..'C'
		})
		
		generateNode(level-1,name..'A',tbl)
		generateNode(level-1,name..'B',tbl)
		generateNode(level-1,name..'C',tbl)
	end,
	
	-- BrightContrast
	function(level,name,tbl)
		local t=
		{
			name=name,
			type="brightcontrast",
			source=name..'A'
		}
		
		-- Decide on brightness source
		if randRange(1,10) <= 5 then
			t.brightness=name..'B'
		else
			t.brightness=rand01()*2-1
		end
		
		-- Decide on contrast threshold and factor
		if randRange(1,10) <= 5 then
			t.contrast_threshold=name..'C'
		else
			t.contrast_threshold=rand01()*2-1
		end
		
		if randRange(1,10)<=5 then
			t.contrast_factor=name..'D'
		else
			t.contrast_factor=rand01()*2-1
		end
		
		table.insert(tbl,1,t)
		generateNode(level-1,name..'A',tbl)
		if type(t.brightness)=='string' then generateNode(level-1,name..'B',tbl) end
		if type(t.contrast_threshold)=='string' then generateNode(level-1,name..'C',tbl) end
		if type(t.contrast_factor)=='string' then generateNode(level-1,name..'D',tbl) end
	end,
	
	--- Combiner
	function(level,name,tbl)
		local numsources=randRange(2,4)
		local t=
		{
			name=name,
			type="combiner",
			operation=randRange(anl.ADD, anl.AVG)
		}
		
		local c
		for c=0,numsources-1,1 do
			local nm="source_"..c
			t[nm]=name..childrenletters[c+1]
		end
		
		table.insert(tbl,1,t)
		
		for c=0,numsources-1,1 do
			generateNode(level-1,name..childrenletters[c+1], tbl)
		end
	end,
	
	-- Cos
	function(level,name,tbl)
		table.insert(tbl,1,{name=name,type="cos", source=name.."A"})
		
		generateNode(level-1,name.."A", tbl)
	end,
	
	-- Fractal
	function(level,name,tbl)
		local numoctaves=randRange(1,8)
		
		-- Insert ac
		table.insert(tbl,1,{name=name,type="autocorrect",source=name.."Fractal", low=-1, high=1})
		
		local t=
		{
			name=name.."Fractal",
			type="fractal",
			fractaltype=randRange(anl.FBM, anl.HYBRIDMULTI),
			basistype=randRange(anl.VALUE, anl.SIMPLEX),
			interptype=randRange(anl.NONE, anl.QUINTIC),
			octaves=numoctaves,
			frequency=rand01()*4,
		}
		
		local overrides={}
		
		local c
		for c=1,numoctaves,1 do
			-- Decide if override
			if randRange(1,10)<=2 then
				overrides[c]=true
				local nm="source_"..(c-1)
				t[nm]=name..childrenletters[c]
			else
				overrides[c]=false
			end
		end
		
		table.insert(tbl,1,t)
		for c=1,numoctaves,1 do
			if overrides[c] then
				generateNode(level-1,name..childrenletters[c], tbl)
			end
		end
	end,
	
	-- functiongradient
	function(level,name,tbl)
		table.insert(tbl,1,
		{
			name=name,
			type="functiongradient",
			source=name..'A',
			axis=randRange(anl.X_AXIS, anl.Y_AXIS),
			spacing=rand01()*0.25
		})
		
		generateNode(level-1,name.."A", tbl)
	end,
	
	-- Gain
	function(level,name,tbl)
	
		-- First, decide if bias should come from constant or source
		if randRange(1,10)<=5 then
			table.insert(tbl,1,
			{
				name=name,
				type="gain",
				source=name..'A',
				gain=rand01()
			})
			generateNode(level-1,name..'A',tbl)
		else
			table.insert(tbl,1,
			{
				name=name,
				type="gain",
				source=name..'A',
				gain=name..'B'
			})
			generateNode(level-1,name..'A',tbl)
			generateNode(level-1,name..'B',tbl)
		end
	end,
	
	-- rotatedomain
	function(level,name,tbl)
		local t=
		{
			name=name,
			type="rotatedomain",
			source=name.."A",
		}
		
		if randRange(1,10)<=5 then
			t.ax=rand01()*2-1
		else
			t.ax=name.."B"
		end
		
		if randRange(1,10)<=5 then
			t.ay=rand01()*2-1
		else
			t.ay=name.."C"
		end
		
		if randRange(1,10)<=5 then
			t.az=rand01()*2-1
		else
			t.az=name.."D"
		end
		
		if randRange(1,10)<=5 then
			t.angle=rand01()*2-1
		else
			t.angle=name.."E"
		end
		
		table.insert(tbl,1,t)
		generateNode(level-1,name.."A",tbl)
		if type(t.ax)=='string' then generateNode(level-1,name.."B",tbl) end
		if type(t.ay)=='string' then generateNode(level-1,name.."C",tbl) end
		if type(t.az)=='string' then generateNode(level-1,name.."D",tbl) end
		if type(t.angle)=='string' then generateNode(level-1,name.."E",tbl) end
	end,
	
	-- scaledomain
	function(level,name,tbl)
		local t=
		{
			name=name,
			type="scaledomain",
			source=name.."A",
		}
		
		if randRange(1,10)<=5 then t.scalex=rand01() else t.scalex=name.."B" end
		if randRange(1,10)<=5 then t.scaley=rand01() else t.scaley=name.."C" end
		
		table.insert(tbl,1,t)
		generateNode(level-1,name.."A",tbl)
		if type(t.scalex)=='string' then generateNode(level-1,name.."B", tbl) end
		if type(t.scaley)=='string' then generateNode(level-1,name.."C",tbl) end
	end,
	
	-- scaleoffset
	function(level,name,tbl)
		local t=
		{
			name=name,
			type="scaleoffset",
			source=name.."A"
		}
		
		if randRange(1,10)<=5 then t.scale=rand01()*2-1 else t.scale=name.."B" end
		if randRange(1,10)<=5 then t.offset=rand01()*2-1 else t.offset=name.."C" end
		
		table.insert(tbl,1,t)
		generateNode(level-1,name.."A",tbl)
		if type(t.scale)=='string' then generateNode(level-1, name.."B", tbl) end
		if type(t.offset)=='string' then generateNode(level-1, name.."C", tbl) end
	end,
	
	-- select
	function(level,name,tbl)
		local t=
		{
			name=name,
			type="select",
			low=name.."A",
			high=name.."B",
			control=name.."C",
		}
		
		if randRange(1,10)<=5 then t.threshold=rand01()*2-1 else t.threshold=name.."D" end
		if randRange(1,10)<=5 then t.falloff=rand01() else t.falloff=name.."E" end
		
		table.insert(tbl,1,t)
		generateNode(level-1,name.."A",tbl)
		generateNode(level-1,name.."B",tbl)
		generateNode(level-1,name.."C",tbl)
		
		if type(t.threshold)=='string' then generateNode(level-1,name.."D",tbl) end
		if type(t.falloff)=='string' then generateNode(level-1,name.."E",tbl) end
	end,
	
	-- sin
	function(level,name,tbl)
		table.insert(tbl,1,{name=name, type="sin", source=name.."A"})
		
		generateNode(level-1,name.."A",tbl)
	end,
	
	-- pow
	function(level,name,tbl)
		local t=
		{
			name=name,
			type="pow",
			source=name.."A"
		}
		
		if randRange(1,10)<=5 then t.power=rand01()*3 else t.power=name.."B" end
		
		table.insert(tbl,1,t)
		generateNode(level-1,name.."A",tbl)
		if type(t.power)=='string' then generateNode(level-1,name.."B", tbl) end
	end,
	
	-- tiers
	function(level,name,tbl)
		local smooth
		if randRange(1,10)<=5 then smooth=true else smooth=false end
		local t=
		{
			name=name,
			type="tiers",
			source=name.."A",
			tiers=randRange(2,8),
			smooth=smooth
		}
		
		table.insert(tbl,1,t)
		generateNode(level-1,name.."A",tbl)
	end,
	
	-- translatedomain
	function(level,name,tbl)
		local t=
		{
			name=name,
			type="translatedomain",
			source=name.."A",
		}
		
		if randRange(1,10)<=5 then t.tx=rand01()*2-1 else t.tx=name.."B" end
		if randRange(1,10)<=5 then t.ty=rand01()*2-1 else t.ty=name.."C" end
		if randRange(1,10)<=5 then t.tz=rand01()*2-1 else t.tz=name.."D" end
		
		table.insert(tbl,1,t)
		generateNode(level-1,name.."A",tbl)
		if type(t.tx)=='string' then generateNode(level-1,name.."B",tbl) end
		if type(t.ty)=='string' then generateNode(level-1,name.."C",tbl) end
		if type(t.tz)=='string' then generateNode(level-1,name.."D",tbl) end
	end,
			
}



function generateNode(level, name, tbl)
	if level<=1 then leafGenerators[randRange(1,#leafGenerators)](level-1, name, tbl)
	else nodeGenerators[randRange(1,#nodeGenerators)](level-1,name,tbl)
	end
end

function rescale(b)
	local w,h=b:width(), b:height()
	
	local x,y,mn,mx
	mn=10000
	mx=-10000
	for x=0,w-1,1 do
		for y=0,h-1,1 do
			local v=b:get(x,y)
			if v>mx then mx=v end
			if v<mn then mn=v end
		end
	end
	
	local scale=1 / (mx-mn);
    local offset=-mn*scale;
	
	for x=0,w-1,1 do
		for y=0,h-1,1 do
			local v=b:get(x,y)
			v=v*scale+offset
			if v<0 then v=0 end
			if v>1 then v=1 end
			b:set(x,y,v)
		end
	end
end

function dothing(levels, nameprefix)
	local t={}
	generateNode(levels,nameprefix,t)
	
	local n=NoiseModuleTree(t)
	
	n:setSeedAll(randInt())
	local a=CArray2Dd(256,256)

ranges=anl.SMappingRanges()
ranges.mapx0=0
ranges.mapx1=2
ranges.mapy0=0
ranges.mapy1=2
anl.map2D(anl.SEAMLESS_NONE, a, n:getFunction(nameprefix), ranges,0)
rescale(a)
anl.saveDoubleArray("F:\\Greyscales\\"..nameprefix..".tga", a)
table.save(t, "F:\\Greyscales\\scripts\\"..nameprefix..".lua")

end