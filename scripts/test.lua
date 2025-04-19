
function OnCreate()
	self.transform = self:GetComponent("Transform2D")
	print("Me acabo de ejecutar al ser agregado a una entidad")
end

function OnUpdate(dt)
	self.transform.x = self.transform.x + 80 * dt
	--self.transform.rot = self.transform.rot + 20 * dt
end
