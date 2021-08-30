#include "CD4051.h"

CD4051::CD4051(Port* A, Port* B, Port* C)
{
	this->A = A;
	this->B = B;
	this->C = C;
		
	this->A->SetAsOutput();
	this->B->SetAsOutput();
	this->C->SetAsOutput();
}

void CD4051::SelectChannel(uint8_t channelNumber)
{
	switch(channelNumber)
	{
		case 0: SelectChannel_0(); break;
		case 1: SelectChannel_1(); break;
		case 2: SelectChannel_2(); break;
		case 3: SelectChannel_3(); break;
		case 4: SelectChannel_4(); break;
		case 5: SelectChannel_5(); break;
		case 6: SelectChannel_6(); break;
		case 7: SelectChannel_7(); break;
	}
}

void CD4051::SelectChannel_0()
{
	this->A->Clear();
	this->B->Clear();
	this->C->Clear();
}

void CD4051::SelectChannel_1()
{
	this->A->Set();
	this->B->Clear();
	this->C->Clear();
}

void CD4051::SelectChannel_2()
{
	this->A->Clear();
	this->B->Set();
	this->C->Clear();
}

void CD4051::SelectChannel_3()
{
	this->A->Set();
	this->B->Set();
	this->C->Clear();
}

void CD4051::SelectChannel_4()
{
	this->A->Clear();
	this->B->Clear();
	this->C->Set();
}

void CD4051::SelectChannel_5()
{
	this->A->Set();
	this->B->Clear();
	this->C->Set();
}

void CD4051::SelectChannel_6()
{
	this->A->Clear();
	this->B->Set();
	this->C->Set();
}

void CD4051::SelectChannel_7()
{
	this->A->Set();
	this->B->Set();
	this->C->Set();
}

CD4051_Inhibit::CD4051_Inhibit(Port* INH)
{
	this->INH = INH;
	
	this->INH->SetAsOutput();
}

void CD4051_Inhibit::EnableOutputs()
{
	this->INH->Clear();
}

void CD4051_Inhibit::DisableOutputs()
{
	this->INH->Set();
}