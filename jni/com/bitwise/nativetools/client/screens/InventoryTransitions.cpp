#include "InventoryTransitions.h"

#include <sstream>

#include "ExtendedInventoryScreen.h"

#include "com/mojang/minecraftpe/client/gui/screen/Screen.h"
#include "com/mojang/minecraftpe/client/gui/screen/ScreenChooser.h"
#include "com/mojang/minecraftpe/client/MinecraftClient.h"

std::shared_ptr<Touch::TButton> InventoryTransitions::forwardButton = NULL;
std::shared_ptr<Touch::TButton> InventoryTransitions::backButton = NULL;

int InventoryTransitions::currentPage = -1;

std::vector<std::shared_ptr<ExtendedInventoryScreen>> InventoryTransitions::pages;

void InventoryTransitions::init(Screen* self)
{
	if(!forwardButton)
	{
		forwardButton = std::make_shared<Touch::TButton>(0, ">", self->mcClient, false, 0x7FFFFFFF);
		forwardButton->init(self->mcClient);
	}
	
	if(!backButton)
	{
		backButton = std::make_shared<Touch::TButton>(1, "<", self->mcClient, false, 0x7FFFFFFF);
		backButton->init(self->mcClient);
	}
	
	if(pages.empty())
	{
		pages.emplace_back(std::make_shared<ExtendedInventoryScreen>(*(self->mcClient)));
		pages.emplace_back(std::make_shared<ExtendedInventoryScreen>(*(self->mcClient)));
	}
	
	self->buttonList.emplace_back(forwardButton);
	self->buttonList.emplace_back(backButton);
}

void InventoryTransitions::setupPositions(Screen* self)
{
	forwardButton->xPosition = self->width - 25;
	forwardButton->yPosition = self->height - 20;
	forwardButton->width = 20;
	forwardButton->height = 20;
	
	backButton->xPosition = self->width - 50;
	backButton->yPosition = self->height - 20;
	backButton->width = 20;
	backButton->height = 20;
}

void InventoryTransitions::render(Screen* self, int i1, int i2, float f1)
{
	std::ostringstream pageNum;
	pageNum << currentPage + 2 << " / " << pages.size() + 1;
	self->drawString(self->font, pageNum.str(), 5, self->height - 15, Color::WHITE);
}

void InventoryTransitions::_buttonClicked(Screen* self, Button& button)
{
	if(button.id == forwardButton->id)
		pushNextScreen(self);
	
	if(button.id == backButton->id)
		pushPreviousScreen(self);
}

void InventoryTransitions::pushNextScreen(Screen* self)
{
	if(pages[currentPage + 1])
	{
		self->mcClient->getScreenChooser()->_pushScreen(pages[currentPage + 1], false);
		currentPage++;
	}
}

void InventoryTransitions::pushPreviousScreen(Screen* self)
{
	if(currentPage >= 0)
	{
		self->mcClient->getScreenChooser()->popScreen(*self, 1);
		currentPage--;
	}
	
}