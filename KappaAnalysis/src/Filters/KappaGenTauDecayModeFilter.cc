#include "Artus/KappaAnalysis/interface/Filters/KappaGenTauDecayModeFilter.h"

std::string KappaGenTauDecayModeFilter::GetFilterId() const {
	return "KappaGenTauDecayModeFilter";
}


void KappaGenTauDecayModeFilter::Init( KappaSettings const& settings)
{
	FilterBase<KappaTypes>::Init(settings);
	m_settingsDecayMode = settings.GetKappaGenTauDecayMode();
}

bool KappaGenTauDecayModeFilter::DoesEventPass(KappaEvent const& event, KappaProduct const& product,
                                       KappaSettings const& settings) const 
{
	std::string m_decayMode = ""; 
	assert(event.m_genTaus->size() >= 2);
	
	KGenTau tau1 = event.m_genTaus->at(0);
	KGenTau tau2 = event.m_genTaus->at(1);
	
	if ((tau1.isElectronicDecay() && tau2.isMuonicDecay()) || (tau2.isElectronicDecay() && tau1.isMuonicDecay())) m_decayMode = "EM";
	else if ((tau1.isElectronicDecay() && tau2.isHadronicDecay()) || (tau2.isElectronicDecay() && tau1.isHadronicDecay())) m_decayMode = "ET";
	else if ((tau1.isMuonicDecay() && tau2.isHadronicDecay()) || (tau2.isMuonicDecay() && tau1.isHadronicDecay())) m_decayMode = "MT";
	else if (tau2.isHadronicDecay() && tau1.isHadronicDecay()) m_decayMode = "TT";
	
	if (m_decayMode == m_settingsDecayMode) return true;
	else return false;
}
