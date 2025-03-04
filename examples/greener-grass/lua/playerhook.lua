if Player then
    Hooks:PostHook(Player, "PatchTaskInventory", "GreenerGrass__Player:PatchTaskInventory", function(self)
        DisplayMessage("Greener Grass","Wow! thats some green grass!")
    end)
end