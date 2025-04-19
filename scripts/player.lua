
function OnCreate()
	-- determina si el jugador esta corriendo o no
	-- se activa cuando el jugador pulsa l-shift
	self.isRunning = false
	self.isRolling = false
	self.speed = 60

	self.transform = self:GetComponent("Transform2D")
	self.sprite_render = self:GetComponent("SpriteRender")
	self.transform.sxy = 4.0
	self:PlayAnimation("idle")

	self.hair = GetEntityByTag("hair")
	self.hair_t = self.hair:GetComponent("Transform2D")
	self.hair_sr = self.hair:GetComponent("SpriteRender")
	self.hair_t.sxy = 4.0
	self.hair:PlayAnimation("idle")

	self.tools = GetEntityByTag("tools")
	self.tools_t = self.tools:GetComponent("Transform2D")
	self.tools_sr = self.tools:GetComponent("SpriteRender")
	self.tools_t.sxy = 4.0
	self.tools:PlayAnimation("idle")
end

function OnEndAnimation(animation)
	if animation == "roll" then
		self:PlayAnimation("idle")
		self.hair:PlayAnimation("idle")
		self.tools:PlayAnimation("idle")
		self.isRolling = false
	end
end

function OnUpdate(dt)

	if KeyDownOnce(Input.KEY_Y) then
		self:Destroy(self.id)
	end

	if KeyDownOnce(Input.KEY_F) then
		CreateEntity2D("prueba", "prueba")
		local ent = GetEntityByTag("prueba")
		ent:AddComponent("SpriteRender", {
			sprite = "player"
		})
		ent:AddComponent("Animation", {
			sprite = "player"
		})
		ent:AddComponent("LuaScript", {
			module = "scripts/test.lua"
		})
		ent:GetComponent("Transform2D").sxy = 4.0
		ent:PlayAnimation("run")
	end

	if KeyDownOnce(Input.KEY_Q) then
		self:Quit(1)
	end

	if KeyDown(Input.KEY_LEFT_SHIFT) then
		self.isRunning = true
		self.speed = 120
	end
	if KeyUpOnce(Input.KEY_LEFT_SHIFT) then
		self.isRunning = false
		self.speed = 60
	end

	if KeyDownOnce(Input.KEY_LEFT_CONTROL) then
		self.isRolling = true
		self:PlayAnimation("roll")
		self.hair:PlayAnimation("roll")
		self.tools:PlayAnimation("roll")
	end
	
	if KeyDown(Input.KEY_D) then
		self.transform.x = self.transform.x + self.speed * dt
		self.sprite_render.flipx = false
		self.hair_sr.flipx = false
		self.tools_sr.flipx = false
		if not self.isRolling then
			if self.isRunning then
				self:PlayAnimation("run")
				self.hair:PlayAnimation("run")
				self.tools:PlayAnimation("run")
			else
				self:PlayAnimation("walk")
				self.hair:PlayAnimation("walk")
				self.tools:PlayAnimation("walk")
			end
		end
	end
	if KeyDown(Input.KEY_A) then
		self.transform.x = self.transform.x - self.speed * dt
		self.sprite_render.flipx = true
		self.hair_sr.flipx = true
		self.tools_sr.flipx = true
		if not self.isRolling then
			if self.isRunning then
				self:PlayAnimation("run")
				self.hair:PlayAnimation("run")
				self.tools:PlayAnimation("run")
			else
				self:PlayAnimation("walk")
				self.hair:PlayAnimation("walk")
				self.tools:PlayAnimation("walk")
			end
		end
	end
	if KeyDown(Input.KEY_W) then
		self.transform.y = self.transform.y - self.speed * dt
		if not self.isRolling then
			if self.isRunning then
				self:PlayAnimation("run")
				self.hair:PlayAnimation("run")
				self.tools:PlayAnimation("run")
			else
				self:PlayAnimation("walk")
				self.hair:PlayAnimation("walk")
				self.tools:PlayAnimation("walk")
			end
		end
	end
	if KeyDown(Input.KEY_S) then
		self.transform.y = self.transform.y + self.speed * dt
		if not self.isRolling then
			if self.isRunning then
				self:PlayAnimation("run")
				self.hair:PlayAnimation("run")
				self.tools:PlayAnimation("run")
			else
				self:PlayAnimation("walk")
				self.hair:PlayAnimation("walk")
				self.tools:PlayAnimation("walk")
			end
		end
	end

	if KeyUpOnce(Input.KEY_D) then
		self:PlayAnimation("idle")
		self.hair:PlayAnimation("idle")
		self.tools:PlayAnimation("idle")
	end
	if KeyUpOnce(Input.KEY_A) then
		self:PlayAnimation("idle")
		self.hair:PlayAnimation("idle")
		self.tools:PlayAnimation("idle")
	end
	if KeyUpOnce(Input.KEY_W) then
		self:PlayAnimation("idle")
		self.hair:PlayAnimation("idle")
		self.tools:PlayAnimation("idle")
	end
	if KeyUpOnce(Input.KEY_S) then
		self:PlayAnimation("idle")
		self.hair:PlayAnimation("idle")
		self.tools:PlayAnimation("idle")
	end
end

function OnDestroy()
	print("He sido destruido pipipipipipipi")
end

function OnCollision(other)
end