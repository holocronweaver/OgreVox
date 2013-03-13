require 'class'
require 'tablesaveload'

MaxSources=20

NoiseModuleTree=class(function(a,declaration)
	a.moduletable={}
	a.libraries={}
	a.seedablemodules={}
	if declaration~=nil then
		a:parseDeclaration(declaration)
	end
end)

function NoiseModuleTree:getFunction(name)
	return self.moduletable[name]
end

-- Functions to parse module parameters

-- Function to verify the existence and type a numeric parameter, with a warning and a default
function NoiseModuleTree:verifyNumberWarn(module, name, default)
	local v=module[name]
	if v==nil or type(v)~="number" then
		print("Warning: Parameter "..name.." of module "..module.name.." not correctly specified. Defaulting to "..default)
		return default
	end
	
	print("Parameter "..name.." of module "..module.name..": "..v)
	return v
end

-- Function to verify the existence and type of a numeric parameter, with an error
function NoiseModuleTree:verifyNumberError(module,name)
	local v=module[name]
	if v==nil or type(v)~="number" then
		print("Error: Parameter "..name.." of module "..module.name.." not specified.")
		return false
	end
	
	print("Parameter "..name.." of module "..module.name..": "..v)
	return v
end

function NoiseModuleTree:verifyNumber(module,name,default)
	if default==nil then return self:verifyNumberError(module,name)
	else return self:verifyNumberWarn(module,name,default)
	end
end

-- Function to verify a boolean parameter, with warn and default
function NoiseModuleTree:verifyBoolean(module,name,default)
	local v=module[name]
	if v~=true and v~=false then
		print("Warning: Boolean parameter "..name.." of module "..module.name.." not specified. Defaulting to "..tostring(default))
		return(default)
	end
	
	print("Parameter "..name.." of module "..module.name..": "..tostring(v))
	return v
end

-- Function to verify a string parameter
function NoiseModuleTree:verifyStringError(module, name)
	local v=module[name]
	if v==nil or type(v)~="string" then
		print("Error: String parameter "..name.." of module "..module.name.." not specified.")
		return false
	end
	
	print("Parameter "..name.." of module "..module.name..": "..v)
	return v
end

function NoiseModuleTree:verifyStringWarn(module,name,default)
	local v=module[name]
	if v==nil or type(v)~="string" then
		print("Warning: String parameter "..name.." of module "..module.name.." not specified. Defaulting to "..default)
		return default
	end
	
	print("Parameter "..name.." of module "..module.name..": "..v)
	return v
end

function NoiseModuleTree:verifyString(module,name,default)
	if default==nil then return self:verifyStringError(module,name)
	else return self:verifyStringWarn(module,name,default)
	end
end

function NoiseModuleTree:verifyScalarParameter(module,name)
	local v=module[name]
	if v==nil then print("ScalarParameter "..name.." not specified, defaulting to built-in default.") return nil end
	if type(v)=='number' then
		return v
	end
	if type(v)=='string' then
		local m=self.moduletable[v]
		if m==nil then print("Error: module "..v.." (set as scalar parameter) does not exist.") return nil end
		return m
	end
	print("ScalarParameter "..name.." not specified, defaulting to built-in default.")
	return nil
end

function NoiseModuleTree:verifyRGBAParameter(module,name)
	local v=module[name]
	if v==nil then
		print("RGBAParameter "..name.." not specified, defaulting to built-in default.")
		return nil
	end
	if type(v)=='table' then
		if v[1]==nil or v[2]==nil or v[3]==nil or v[4]==nil then
			print("Warning: RGBA constant in RGBAParameter "..name.." malformed; defaulting to built-in default.")
			return nil
		end
		return v
	end
	if type(v)=='string' then
		local m=self.moduletable[v]
		if m==nil then
			print("Warning: module "..v.." set as RGBAParameter does not exist.")
			return nil
		end
		return m
	end
	print("RGBAParameter "..name.." not specified. Defaulting to built-in default.")
	return nil
end

-- Function to verify a source module
function NoiseModuleTree:verifyModuleError(module, name)
	local v=module[name]
	if v==nil or type(v)~="string" then
		print("Error: Module parameter "..name.." of module "..module.name.." not specified.")
		return false
	end
	
	local m=self.moduletable[v]
	if m==nil then
		print("Error: Module parameter "..v.." specified for module "..module.name.." does not exist.")
		return false
	end
	
	print("Parameter "..name.." of module "..module.name..": "..v)
	return m
end

function NoiseModuleTree:verifyModule(module, name)
	local v=module[name]
	if v==nil then return nil end
	local m=self.moduletable[v]
	return m
end

function NoiseModuleTree:parseDeclaration(declaration)
	local i
	for i=1,#declaration,1 do
		if self:parseModule(declaration[i])==false then
			-- Error condition
			print("Aborting")
			return
		end
	end
	print("Declaration parsed. "..(#declaration).." modules.")
end

function NoiseModuleTree:setSeedFunction(name,seed)
	local f=self:getFunction(name)
	if f then f:setSeed(seed) end
end

function NoiseModuleTree:setSeedAll(startseed)
	local i,j
	local k=0
	for i,j in pairs(self.moduletable) do
		print("Seeding "..i)
		j:setSeed(startseed+k*10)
		k=k+1
	end
end

function NoiseModuleTree:parseModule(module)
	if module==nil then
		print("Error: nil module table passed to parseModule")
		return false
	end
	
	-- Check name
	if module.name==nil then
		print("Error: module name not specified")
		return false
	end
	
	if self.moduletable[module.name] ~= nil then
		print("Error: Module "..module.name.." already exists.")
		return false
	end
	
	-- Check type
	
	if module.type==nil then
		print("Error: module type not specified.")
		return false
	end
	
	-- The name of the module type directly references the name of a function to parse the module
	if self[module.type]==nil then
		print("Error: module type "..module.type.." unsupported.")
		return false
	end
	
	return self[module.type](self,module)
end

function NoiseModuleTree:gatherMultipleSources(module,func)
	-- Parse the source_n entries of module, check and obtain the modules and set the sources
	for i=0,MaxSources-1,1 do
		local s="source_"..i
		if module[s]~=nil then
			if self.moduletable[module[s]]==nil then
				print("Error: Module "..module[s].." specified as source to module "..module.name.." does not exist.")
				return false
			end
			if module[s]==module.name then
				print("Error: Module "..module.name.." specified as source for itself; will result in infinite loop condition.")
				return false
			end
			func:setSource(i,self.moduletable[module[s]])
			print("Module "..module[s].."set as source "..i.." of module "..module.name)
		end
	end
end

function NoiseModuleTree:gatherMultipleSourcesFractal(module,func)
	-- Parse the source_n entries of module, check and obtain the modules and set the sources
	for i=0,MaxSources-1,1 do
		local s="source_"..i
		if module[s]~=nil then
			if self.moduletable[module[s]]==nil then
				print("Error: Module "..module[s].." specified as source to module "..module.name.." does not exist.")
				return false
			end
			if module[s]==module.name then
				print("Error: Module "..module.name.." specified as source for itself; will result in infinite loop condition.")
				return false
			end
			func:overrideSource(i,self.moduletable[module[s]])
			print("Module "..module[s].."set as source "..i.." of module "..module.name)
		end
	end
end


-- Module parsers

-- Library function
function NoiseModuleTree:library(module)
	-- Library loads a file specified by a path. The file is loaded line by line, and each line is parsed using parseModule()
	local pathname=self:verifyStringError(module, "path")
	if pathname==nil then return nil end
	
	-- Load the file given by path/name
	local filename=pathname..module.libraryname..".lua"
	--local infile=io.open(pathname..module.libraryname..".lua")
	--if infile==nil then
		--print("Error: Library file "..pathname..module.libraryname.." does not exist.")
		--return nil
	--end
	
	--local l
	--local lib=NoiseModuleTree()
	--for l in infile:lines() do
		--local t=table.load(l)
		--self:parseModule(t)
		--if lib:parseModule(t)==nil then
			--print("Error: Malformed module declaration in library "..pathname..module.libraryname)
			--infile:close()
			--return nil
		--end
	--end
	--infile:close()
	
	local l,err=table.load(filename)
	if l==nil then print(err) return nil end
	
	self.libraries[module.libraryname]=NoiseModuleTree(l)
	self.moduletable[module.name]=self.libraries[module.libraryname]:getFunction(module.libraryname)
end


-- AutoCorrect
function NoiseModuleTree:autocorrect(module)
	-- Parameters:
	--   source: module
	--   low,high: double
	local source=self:verifyModuleError(module,"source")
	if source==false then return nil end
	local low=self:verifyNumber(module, "low", -1.0)
	local high=self:verifyNumber(module, "high", 1.0)
	
	local f=anl.CImplicitAutoCorrect()
	f:setRange(low,high)
	f:setSource(source)
	self.moduletable[module.name]=f
	return true
end


-- BasisFunction
function NoiseModuleTree:basisfunction(module)
	-- Parameters
	-- basis, interp: int
	local basis=self:verifyNumber(module,"basis",anl.GRADIENT)
	local interp=self:verifyNumber(module,"interp",anl.QUINTIC)
	
	local f=anl.CImplicitBasisFunction(basis,interp)
	self.moduletable[module.name]=f
	return true
end

-- Bias
function NoiseModuleTree:bias(module)
	-- Parameters:
	--   source: scalarparameter
	--   bias: scalarparameter
	local source=self:verifyScalarParameter(module,"source")
	local bias=self:verifyScalarParameter(module,"bias")
	
	local f=anl.CImplicitBias(0)
	
	if source then f:setSource(source) end
	if bias then f:setBias(bias) end
	
	self.moduletable[module.name]=f
	return true
end

-- Blend
function NoiseModuleTree:blend(module)
	-- Parameters:
	-- source,low,high: scalarparameters
	
	local source=self:verifyScalarParameter(module,"control")
	local low=self:verifyScalarParameter(module,"low")
	local high=self:verifyScalarParameter(module,"high")
	
	local f=anl.CImplicitBlend()
	
	if source then f:setControlSource(source) end
	if low then f:setLowSource(low) end
	if high then f:setHighSource(high) end
	
	self.moduletable[module.name]=f
	return true
end

-- BrightContrast
function NoiseModuleTree:brightcontrast(module)
	-- Parameters
	-- source, bright, threshold, factor: scalarparameters
	local source=self:verifyScalarParameter(module,"source")
	local bright=self:verifyScalarParameter(module,"brightness")
	local threshold=self:verifyScalarParameter(module,"contrast_threshold")
	local factor=self:verifyScalarParameter(module, "contrast_factor")
	
	local f=anl.CImplicitBrightContrast()
	
	if source then f:setSource(source) end
	if bright then f:setBrightness(bright) end
	if threshold then f:setContrastThreshold(threshold) end
	if factor then f:setContrastFactor(factor) end
	self.moduletable[module.name]=f
	return true
end

-- Cache
function NoiseModuleTree:cache(module)
	-- Parameters
	-- source: module
	local source=self:verifyModuleError(module,"source")
	if source==nil then return false end
	
	local f=anl.CImplicitCache()
	f:setSource(source)
	self.moduletable[module.name]=f
	return true
end

-- Cellular generator. Not really a module, per se...
function NoiseModuleTree:cellulargenerator(module)
	-- Parameters
	-- seed (optional): int
	
	local f=anl.CCellularGenerator()
	
	self.moduletable[module.name]=f
	if module.seed then
		f:setSeed(module.seed)
	end
	return true
end

-- Cellular
function NoiseModuleTree:cellular(module)
	-- Parameters
	-- f1,f2,f3,f4: Coefficients
	-- source: CellularGenerator
	local source=self:verifyModuleError(module,"source")
	if source==nil then return false end
	
	local f1,f2,f3,f4=self:verifyNumber(module,"f1",0.0), self:verifyNumber(module,"f2",0.0), self:verifyNumber(module,"f3",0.0), self:verifyNumber(module,"f4",0.0)
	
	local f=anl.CImplicitCellular()
	f:setCellularSource(source)
	f:setCoefficients(f1,f2,f3,f4)
	self.moduletable[module.name]=f
	return true
end

-- Clamp
function NoiseModuleTree:clamp(module)
	-- Parameters
	-- source: module
	-- low,high: number
	local source=self:verifyModuleError(module,"source")
	if source==nil then return false end
	
	local low=self:verifyNumber(module,"low",-1.0)
	local high=self:verifyNumber(module,"high",1.0)
	
	local f=anl.CImplicitClamp(low,high)
	f:setSource(source)
	self.moduletable[module.name]=f
	return true
end

-- Combiner
function NoiseModuleTree:combiner(module)
	local type=self:verifyNumber(module,"operation", anl.ADD)
	
	self.moduletable[module.name]=anl.CImplicitCombiner(type)
	self:gatherMultipleSources(module,self.moduletable[module.name])
	print("Module "..module.name.."(combiner,"..type..") created.")
	return true
end

-- Constant
function NoiseModuleTree:constant(module)
	local constant=self:verifyNumber(module,"constant",1.0)
		
	self.moduletable[module.name]=anl.CImplicitConstant(constant)
	print("Module "..module.name.."(constant,"..constant..") created.")
	return true
end

-- Cos
function NoiseModuleTree:cos(module)
	local source=self:verifyScalarParameter(module, "source")
	
	local f=anl.CImplicitCos()
	if source then f:setSource(source) end
	self.moduletable[module.name]=f
	return true
end

function NoiseModuleTree:curve(module)
	local source=self:verifyScalarParameter(module, "source")
	
	local f=anl.CImplicitCurve()
	if module.points then
		local c
		for c=1,#module.points,1 do
			f:pushPoint(module.points[c][1], module.points[c][2])
		end
	end
	
	if source then f:setSource(source) end
	self.moduletable[module.name]=f
	return true
end

-- Floor
function NoiseModuleTree:floor(module)
	local source=self:verifyScalarParameter(module, "source")
	
	local f=anl.CImplicitFloor()
	if source then f:setSource(source) end
	self.moduletable[module.name]=f
	return true
end

-- RGBAExtractChannel
function NoiseModuleTree:extractchannel(module)
	-- Parameters
	-- source: RGBAParameter
	-- channel: int
	local source=self:verifyRGBAParameter(module,"source")
	local channel=self:verifyNumber(module,"channel",anl.RED)
	
	local f=anl.CImplicitExtractRGBAChannel()
	f:setChannel(channel)
	if source~=nil and type(source)=='table' then
		f:setSource(source[1],source[2],source[3],source[4])
	else
		f:setSource(source)
	end
	
	self.moduletable[module.name]=f
	return true
end

-- Fractal
function NoiseModuleTree:fractal(module)
	local type=self:verifyNumber(module,"fractaltype",anl.FBM)
	local basis=self:verifyNumber(module,"basistype",anl.GRADIENT)
	local interp=self:verifyNumber(module,"interptype",anl.QUINTIC)
	local octaves=self:verifyNumber(module,"octaves", 8)
	local freq=self:verifyNumber(module,"frequency",1.0)
	
	
	-- TODO: Implement setters for Lacunarity, Offset, Gain, H
	local f=anl.CImplicitFractal(type,basis,interp)
	f:setNumOctaves(octaves)
	f:setFrequency(freq)
	
	-- Specify any overrides
	print("Module "..module.name..": Check for octave overrides...")
	self:gatherMultipleSourcesFractal(module,f)
	
	if module.seed then f:setSeed(module.seed) end
	self.moduletable[module.name]=f
	print("Module "..module.name.."(fractal) created.")
	return true
end

-- FunctionGradient
function NoiseModuleTree:functiongradient(module)
	local source=self:verifyScalarParameter(module,"source")
	local axis=self:verifyNumber(module,"axis",anl.X_AXIS)
	local spacing=self:verifyNumber(module,"spacing",0.01)
	
	local f=anl.CImplicitFunctionGradient()
	if source then f:setSource(source) end
	f:setAxis(axis)
	f:setSpacing(spacing)
	self.moduletable[module.name]=f
	return true
end

-- Gain
function NoiseModuleTree:gain(module)
	-- Parameters:
	--   source: scalarparameter
	--   bias: scalarparameter
	local source=self:verifyScalarParameter(module,"source")
	local bias=self:verifyScalarParameter(module,"gain")
	if bias==nil then bias=0 end
	
	local f=anl.CImplicitGain(0)
	if bias then f:setGain(bias) end
	
	if source then f:setSource(source) end
	
	self.moduletable[module.name]=f
	return true
end

-- Gradient
function NoiseModuleTree:gradient(module)
	local x1,y1,z1,w1,u1,v1, x2,y2,z2,w2,u2,v2=0,0,0,0,0,0,0,0,0,0,0,0
	
	if module.x1~=nil then x1=module.x1 end
	if module.x2~=nil then x2=module.x2 end
	if module.y1~=nil then y1=module.y1 end
	if module.y2~=nil then y2=module.y2 end
	if module.z1~=nil then z1=module.z1 end
	if module.z2~=nil then z2=module.z2 end
	if module.w1~=nil then w1=module.w1 end
	if module.w2~=nil then w2=module.w2 end
	if module.u1~=nil then u1=module.u1 end
	if module.u2~=nil then u2=module.u2 end
	if module.v1~=nil then v1=module.v1 end
	if module.v2~=nil then v2=module.v2 end
	
	self.moduletable[module.name]=anl.CImplicitGradient()
	self.moduletable[module.name]:setGradient(x1,x2,y1,y2,z1,z2,w1,w2,u1,u2,v1,v2)
	print("Module "..module.name.."(gradient) created.")
	return true
end

function NoiseModuleTree:magnitude(module)
	local x=self:verifyScalarParameter(module,"x")
	local y=self:verifyScalarParameter(module,"y")
	local z=self:verifyScalarParameter(module,"z")
	local w=self:verifyScalarParameter(module,"w")
	local u=self:verifyScalarParameter(module,"u")
	local v=self:verifyScalarParameter(module,"v")
	
	self.moduletable[module.name]=anl.CImplicitMagnitude()
	if x then self.moduletable[module.name]:setX(x) end
	if y then self.moduletable[module.name]:setY(y) end
	if z then self.moduletable[module.name]:setZ(z) end
	if w then self.moduletable[module.name]:setW(w) end
	if u then self.moduletable[module.name]:setU(u) end
	if v then self.moduletable[module.name]:setV(v) end
	return true
end

function NoiseModuleTree:normalizecoords(module)
	local src=self:verifyScalarParameter(module,"source")
	local len=self:verifyScalarParameter(module,"length")
	self.moduletable[module.name]=anl.CImplicitNormalizeCoords()
	if src then self.moduletable[module.name]:setSource(src) end
	if len then self.moduletable[module.name]:setLength(len) end
	return true
end

-- RGBADotProduct
function NoiseModuleTree:rgbadotproduct(module)
	local s1=self:verifyRGBAParameter(module,"source1")
	local s2=self:verifyRGBAParameter(module,"source2")
	
	local f=anl.CImplicitRGBADotProduct()
	if s1 then
		if type(s1)=='table' then
			f:setSource1(s1[1],s1[2],s1[3],s1[4])
		else
			f:setSource1(s1)
		end
	end
	
	if s2 then
		if type(s2)=='table' then
			f:setSource2(s2[1], s2[2], s2[3], s2[4])
		else
			f:setSource2(s2)
		end
	end
	
	self.moduletable[module.name]=f
	return true
end

-- RotateDomain
function NoiseModuleTree:rotatedomain(module)
	-- Parameters
	-- ax,ay,az,angle: scalarparameters
	-- source: scalarparameter
	local ax=self:verifyScalarParameter(module,"ax")
	local ay=self:verifyScalarParameter(module,"ay")
	local az=self:verifyScalarParameter(module,"az")
	local angle=self:verifyScalarParameter(module,"angle")
	local source=self:verifyModuleError(module,"source")
	if source==nil then return false end
	
	local f=anl.CImplicitRotateDomain(0,0,0,0)
	if ax then f:setAxisX(ax) end
	if ay then f:setAxisY(ay) end
	if az then f:setAxisZ(az) end
	if angle then f:setAngle(angle) end
	f:setSource(source)
	
	self.moduletable[module.name]=f
	return true
end

-- ScaleDomain
function NoiseModuleTree:scaledomain(module)
	-- Parameters
	-- scalex,scaley,scalez,scalew,scaleu,scalev: scalarparameters
	-- source: module
	local source=self:verifyModuleError(module,"source")
	if source==nil then return false end
	
	local scalex=self:verifyScalarParameter(module,"scalex")
	local scaley=self:verifyScalarParameter(module,"scaley")
	local scalez=self:verifyScalarParameter(module,"scalez")
	local scalew=self:verifyScalarParameter(module,"scalew")
	local scaleu=self:verifyScalarParameter(module,"scaleu")
	local scalev=self:verifyScalarParameter(module,"scalev")
	
	local f=anl.CImplicitScaleDomain()
	f:setSource(source)
	if scalex then f:setXScale(scalex) end
	if scaley then f:setYScale(scaley) end
	if scalez then f:setZScale(scalez) end
	if scalew then f:setWScale(scalew) end
	if scaleu then f:setUScale(scaleu) end
	if scalev then f:setVScale(scalev) end
	
	self.moduletable[module.name]=f
	return true
end

function NoiseModuleTree:sawtooth(module)
	local source=self:verifyModuleError(module, "source")
	local period=self:verifyScalarParameter(module, "period")
	
	local f=anl.CImplicitSawtooth(1)
	if source then f:setSource(source) end
	if period then f:setPeriod(period) end
	self.moduletable[module.name]=f
	return true
end
	

-- ScaleOffset
function NoiseModuleTree:scaleoffset(module)
	-- Parameters
	-- source: module
	-- scale,offset: scalarparameters
	local source=self:verifyModuleError(module,"source")
	if source==nil then return false end
	
	local scale=self:verifyScalarParameter(module,"scale")
	local offset=self:verifyScalarParameter(module,"offset")
	
	local f=anl.CImplicitScaleOffset(1,0)
	f:setSource(source)
	if scale then f:setScale(scale) end
	if offset then f:setOffset(offset) end
	self.moduletable[module.name]=f
	return true
end

-- Select
function NoiseModuleTree:select(module)
	-- Parameters
	-- low,high,control,threshold,falloff: scalar parameters
	local low=self:verifyScalarParameter(module,"low")
	local high=self:verifyScalarParameter(module,"high")
	local control=self:verifyScalarParameter(module,"control")
	local threshold=self:verifyScalarParameter(module,"threshold")
	local falloff=self:verifyScalarParameter(module,"falloff")
	
	local f=anl.CImplicitSelect()
	if low then f:setLowSource(low) end
	if high then f:setHighSource(high) end
	if control then f:setControlSource(control) end
	if threshold then f:setThreshold(threshold) end
	if falloff then f:setFalloff(falloff) end
	
	self.moduletable[module.name]=f
	return true
end

function NoiseModuleTree:sin(module)
	local source=self:verifyScalarParameter(module,"source")
	
	local f=anl.CImplicitSin()
	if source then f:setSource(source) end
	
	self.moduletable[module.name]=f
	return true
end

-- Sphere
function NoiseModuleTree:sphere(module)
	-- Parameters
	-- cx,cy,cz,cw,cu,cv,radius: scalarparameter
	local cx=self:verifyScalarParameter(module,"cx")
	local cy=self:verifyScalarParameter(module,"cy")
	local cz=self:verifyScalarParameter(module,"cz")
	local cw=self:verifyScalarParameter(module,"cw")
	local cu=self:verifyScalarParameter(module,"cu")
	local cv=self:verifyScalarParameter(module,"cv")
	
	local radius=self:verifyScalarParameter(module,"radius")
	
	local f=anl.CImplicitSphere()
	if cx then f:setCenterX(cx) end
	if cy then f:setCenterY(cy) end
	if cz then f:setCenterZ(cz) end
	if cw then f:setCenterW(cw) end
	if cu then f:setCenterU(cu) end
	if cv then f:setCenterV(cv) end
	if radius then f:setRadius(radius) end
	
	self.moduletable[module.name]=f
	return true
end

-- Sqrt
function NoiseModuleTree:pow(module)
	local source=self:verifyScalarParameter(module,"source")
	local power=self:verifyScalarParameter(module,"power")
	
	local f=anl.CImplicitPow()
	if source then f:setSource(source) end
	if power then f:setPower(power) end
	
	self.moduletable[module.name]=f
	return true
end

-- Tiers
function NoiseModuleTree:tiers(module)
	local source=self:verifyScalarParameter(module,"source")
	local numtiers=self:verifyNumber(module,"tiers",2)
	local smooth=self:verifyBoolean(module,"smooth", true)
	
	local f=anl.CImplicitTiers(numtiers, smooth)
	if source then f:setSource(source) end
	self.moduletable[module.name]=f
	return true
end

-- TranslateDomain
function NoiseModuleTree:translatedomain(module)
	-- Parameters
	-- tx,ty,tz,tw,tu,tv: scalarparameter
	local tx=self:verifyScalarParameter(module,"tx")
	local ty=self:verifyScalarParameter(module,"ty")
	local tz=self:verifyScalarParameter(module,"tz")
	local tw=self:verifyScalarParameter(module,"tw")
	local tu=self:verifyScalarParameter(module,"tu")
	local tv=self:verifyScalarParameter(module,"tv")
	
	local source=self:verifyScalarParameter(module,"source")
	
	local f=anl.CImplicitTranslateDomain()
	if tx then f:setXAxisSource(tx) end
	if ty then f:setYAxisSource(ty) end
	if tz then f:setZAxisSource(tz) end
	if tw then f:setWAxisSource(tw) end
	if tu then f:setUAxisSource(tu) end
	if tv then f:setVAxisSource(tv) end
	if source then f:setSource(source) end
	
	self.moduletable[module.name]=f
	return true
end

function NoiseModuleTree:triangle(module)
	local source=self:verifyScalarParameter(module, "source")
	local period=self:verifyScalarParameter(module, "period")
	local offset=self:verifyScalarParameter(module, "offset")
	
	local f=anl.CImplicitTriangle(1,0.5)
	if source then f:setSource(source) end
	if period then f:setPeriod(period) end
	if offset then f:setOffset(offset) end
	
	self.moduletable[module.name]=f
	return true
end



-- RGBA Modules

-- Blend

function NoiseModuleTree:rgba_blend(module)
	-- Parameters
	-- low,high: RGBAParameter
	-- control: scalarparamter
	local low=self:verifyRGBAParameter(module,"low")
	local high=self:verifyRGBAParameter(module,"high")
	local control=self:verifyScalarParameter(module,"control")
	
	local f=anl.CRGBABlend()
	if low then
		if type(low)=='table' then
			f:setLowSource(low[1],low[2],low[3],low[4])
		else
			f:setLowSource(low)
		end
	end
	if high then
		if type(high)=='table' then
			f:setHighSource(high[1],high[2],high[3],high[4])
		else
			f:setHighSource(high)
		end
	end
	
	if control then f:setControlSource(control) end
	
	self.moduletable[module.name]=f
	return true
end

-- BlendOps
function NoiseModuleTree:rgba_blendops(module)
	-- Parameters
	-- source1,source2: RGBAParameter
	-- blend1,blend2: int
	local source1=self:verifyRGBAParameter(module,"source1")
	local source2=self:verifyRGBAParameter(module,"source2")
	local blend1=self:verifyNumber(module,"blend1",anl.SRC1_ALPHA)
	local blend2=self:verifyNumber(module,"blend2",anl.ONE_MINUS_SRC1_ALPHA)
	
	local f=anl.CRGBABlendOps()
	if source1 then
		if type(source1)=='table' then
			f:setSource1(source1[1], source1[2], source1[3], source1[4])
		else
			f:setSource1(source1)
		end
	end
	
	if source2 then
		if type(source2)=='table' then
			f:setSource2(source2[1],source2[2],source2[3],source2[4])
		else
			f:setSource2(source2)
		end
	end
	
	f:setSrc1Mode(blend1)
	f:setSrc2Mode(blend2)
	
	self.moduletable[module.name]=f
	return true
end

function NoiseModuleTree:rgba_colorops(module)
	local op=self:verifyNumber(module, "operation", anl.SOFTLIGHT)
	local source1=self:verifyRGBAParameter(module,"source1")
	local source2=self:verifyRGBAParameter(module,"source2")
	
	local f=anl.CRGBAColorOps()
	f:setOperation(op)
	if source1 then
		if type(source1)=='table' then
			f:setSource1(source1[1], source1[2], source1[3], source1[4])
		else
			f:setSource1(source1)
		end
	end
	
	if source2 then
		if type(source2)=='table' then
			f:setSource2(source2[1],source2[2],source2[3],source2[4])
		else
			f:setSource2(source2)
		end
	end
	
	self.moduletable[module.name]=f
	return true
end

-- ComposeChannels
function NoiseModuleTree:rgba_composechannels(module)
	-- Parameters
	-- mode: int
	-- r,g,b,a,h,s,v: scalarparamter
	
	local mode=self:verifyNumber(module,"mode",anl.RGB)
	local r,g,b,a
	if mode==anl.HSV then
		r=self:verifyScalarParameter(module,"hue")
		g=self:verifyScalarParameter(module,"sat")
		b=self:verifyScalarParameter(module,"val")
	else
		r=self:verifyScalarParameter(module,"red")
		g=self:verifyScalarParameter(module,"green")
		b=self:verifyScalarParameter(module,"blue")
	end
	
	local a=self:verifyScalarParameter(module,"alpha")
	
	local f=anl.CRGBACompositeChannels()
	
	if r then f:setRedSource(r) end
	if g then f:setGreenSource(g) end
	if b then f:setBlueSource(b) end
	if a then f:setAlphaSource(a) end
	f:setMode(mode)
	self.moduletable[module.name]=f
	return true
end

function NoiseModuleTree:rgba_curve(module)
	local source=self:verifyScalarParameter(module, "source")
	
	local f=anl.CRGBACurve()
	if module.points then
		local c
		for c=1,#module.points,1 do
			f:pushPoint(module.points[c][1], module.points[c][2][1], module.points[c][2][2], module.points[c][2][3], module.points[c][2][4])
		end
	end
	
	if source then f:setSource(source) end
	self.moduletable[module.name]=f
	return true
end

-- HSVtoRGBA
function NoiseModuleTree:rgba_hsvtorgba(module)
	-- Parameters
	-- source: RGBAParameter
	local source=self:verifyRGBAParameter(module,"source")
	
	local f=anl.CRGBAHSVToRGBA()
	if source then
		if type(source)=='table' then
			f:setSource(source[1],source[2],source[3],source[4])
		else
			f:setSource(source)
		end
	end
	
	self.moduletable[module.name]=f
	return true
end

-- ImplicitGrayscale
function NoiseModuleTree:rgba_implicitgrayscale(module)
	-- Parameters
	-- source:scalarparameter
	local source=self:verifyScalarParameter(module,"source")
	
	local f=anl.CRGBAImplicitGrayscale()
	if source then f:setSource(source) end
	
	self.moduletable[module.name]=f
	return true
end

-- Normalize
function NoiseModuleTree:rgba_normalize(module)
	local s=self:verifyRGBAParameter(module,"source")
	
	local f=anl.CRGBANormalize()
	if s then
		if type(s)=='table' then
			f:setSource(s[1],s[2],s[3],s[4])
		else
			f:setSource(s)
		end
	end
	
	self.moduletable[module.name]=f
	return true
end

-- RGBAtoHSV
function NoiseModuleTree:rgba_rgbatohsv(module)
	-- Paramters
	-- source: RGBAParamter
	local s = self:verifyRGBAParameter(module,"source")
	
	local f=anl.CRGBARGBAToHSV()
	if s then
		if type(s)=='table' then
			f:setSource(s[1],s[2],s[3],s[4])
		else
			f:setSource(s)
		end
	end
	
	self.moduletable[module.name]=f
	return true
end

-- RotateColor
function NoiseModuleTree:rgba_rotatecolor(module)
	-- Parameters
	-- ax,ay,az,angle: scalarparamter
	-- source: rgbaparameter
	
	local ax=self:verifyScalarParameter(module,"ax")
	local ay=self:verifyScalarParameter(module,"ay")
	local az=self:verifyScalarParameter(module,"az")
	local angle=self:verifyScalarParameter(module,"angle")
	local s = self:verifyRGBAParameter(module,"source")
	
	local f=anl.CRGBARotateColor()
	if ax then f:setAxisX(ax) end
	if ay then f:setAxisY(ay) end
	if az then f:setAxisZ(az) end
	if angle then f:setAngle(angle) end
	if s then
		if type(s)=='table' then
			f:setSource(s[1],s[2],s[3],s[4])
		else
			f:setSource(s)
		end
	end
	
	self.moduletable[module.name]=f
	return true
end

-- Select
function NoiseModuleTree:rgba_select(module)
	-- Parameters
	-- control,falloff,threshold: scalar
	-- low,high: rgbaparam
	local low=self:verifyRGBAParameter(module,"low")
	local high=self:verifyRGBAParameter(module,"high")
	local control=self:verifyScalarParameter(module,"control")
	local threshold=self:verifyScalarParameter(module,"threshold")
	local falloff=self:verifyScalarParameter(module,"falloff")
	
	local f=anl.CRGBASelect()
	
	if low then
		if type(low)=='table' then
			f:setLowSource(low[1],low[2],low[3],low[4])
		else
			f:setLowSource(low)
		end
	end
	
	if high then
		if type(high)=='table' then
			f:setHighSource(high[1],high[2],high[3],high[4])
		else
			f:setHighSource(high)
		end
	end
	
	if control then f:setControlSource(control) end
	if threshold then f:setThreshold(threshold) end
	if falloff then f:setFalloff(falloff) end
	
	self.moduletable[module.name]=f
	return true
end

	

