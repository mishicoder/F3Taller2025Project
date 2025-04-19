function OnCreate()
	self.camera = self:GetComponent("Camera2D")
	self.player = GetEntityByTag("player")
	self.t = self.player:GetComponent("Transform2D")
end

function OnUpdate(dt)
	if KeyDownOnce(Input.KEY_SPACE) then
		if self:Destroy(self.player.id) then
			print("Entidad destruida")
		else
			print("La entidad no existe")
		end
		--self:Destroy(self.player.id)
		--self:AddComponent("SpriteRender", {
		--	sprite = "player"
		--})
	end

	if KeyDownOnce(Input.KEY_R) then
		self:RemoveComponent("SpriteRender")
	end
	--self.t.x = self.t.x + 60 * dt
	--if KeyDown(Input.KEY_D) then
		--self.t.x = self.t.x + 60 * dt
	--end
end