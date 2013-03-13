-- Linked list
-- Abstract a linked-list class using Lua tables

require 'Class'

LinkedList=class(function(a) a.list=nil end)

function LinkedList.push_front(self, object)
	self.list = {next=self.list, value=object}
end


function LinkedList.findNode(self, object)
	local prev=nil
	local p=self.list
	local success=false
	
	while p~=nil and success==false do
		if p.value==object then
			success=true
		else
			prev=p
			p=p.next
		end
	end
	return success, p, prev
end

function LinkedList.remove(self, object)
	local success, p, prev = self:findNode(object)
	if success==false then return end
	if prev==nil then
		-- Removing node at front of list
		self.list=self.list.next
	else
		prev.next=p.next
	end
end

