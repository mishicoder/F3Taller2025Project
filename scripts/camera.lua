function OnCreate()
	self.camera = self:GetComponent("Camera2D")
	self.player = GetEntityByTag("player")
	self.t = self.player:GetComponent("Transform2D")
end

function OnUpdate(dt)
	--self.t.x = self.t.x + 60 * dt
	--if KeyDown(Input.KEY_D) then
		--self.t.x = self.t.x + 60 * dt
	--end
end