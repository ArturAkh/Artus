
#pragma once

#include "Artus/Core/interface/FilterBase.h"
#include "Artus/KappaAnalysis/interface/KappaTypes.h"

class KappaGenTauDecayModeFilter: public FilterBase<KappaTypes> {
public:

	std::string GetFilterId() const override;

	bool DoesEventPass(KappaEvent const& event, KappaProduct const& product,
	                           KappaSettings const& settings) const override;
	void Init(setting_type const& settings) override;

private:
	std::string m_settingsDecayMode;
};
