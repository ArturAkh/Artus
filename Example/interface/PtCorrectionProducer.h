/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include "Artus/Core/interface/Cpp11Support.h"

#include "TraxTypes.h"

class PtCorrectionProducer: public TraxProducerBase {
public:

	virtual std::string GetProducerId() const ARTUS_CPP11_OVERRIDE
	{
		return "pt_correction";
	}
	
	virtual void InitGlobal(global_setting_type const& globalSettings)  ARTUS_CPP11_OVERRIDE
	{
		ProducerBase<TraxTypes>::InitGlobal(globalSettings);
	}
	
	virtual void InitLocal(setting_type const& settings)  ARTUS_CPP11_OVERRIDE
	{
		ProducerBase<TraxTypes>::InitLocal(settings);
	}

	virtual void ProduceGlobal(TraxEvent const& event,
			TraxProduct & product,
			TraxGlobalSettings const& globalSettings) const
					ARTUS_CPP11_OVERRIDE {

		product.m_floatPtSim_corrected = event.m_floatPtSim
				* globalSettings.GetProducerPtCorrectionFactor();
	}

};
