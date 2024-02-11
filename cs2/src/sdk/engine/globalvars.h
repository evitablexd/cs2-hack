#pragma once

class SubGlobalVariables // idfk what to call it, there are variables in here
{
public:
	PAD(0x8);
	float m_flIntervalPerTick; // 0x0008
	float m_flCurrentTime; //0x000C
	float m_flCurrentTime2; //0x0010
	float somefloat; //0x0014
	float somefloat1; //0x0018
	PAD(0x140);
	char* m_szCurrentMap; //0x0164
	char* m_szCurrentMapName; //0x016C
};

class IGlobalVariables
{
public:
	float m_flRealTime; //0x0000
	int32_t m_nFrameCount; //0x0004
	float m_flFrameTime; //0x0008
	float m_flFrameTime2; //0x000C
	int32_t m_nMaxClients; //0x0010
	PAD(0xC);
	SubGlobalVariables m_SubGlobalVariables; //0x0020
};