
#pragma once

#include <boost/noncopyable.hpp>

#include "Artus/Core/interface/GlobalInclude.h"

/**
   \brief Base class for Producers, that extend the product.
   
   This is a base class for Producers, that extend the product after performing
   some calculations. The function ProduceGlobal is called once per event before
   the pipelines are split. The function ProduceLocal is called once per event
   within a given pipeline. Only one of these functions are called.
*/

template<class TTypes>
class ProducerBase: public boost::noncopyable {
public:

	typedef typename TTypes::event_type event_type;
	typedef typename TTypes::product_type product_type;
	typedef typename TTypes::setting_type setting_type;
	typedef typename TTypes::global_setting_type global_setting_type;

	virtual ~ProducerBase() {

	}

	virtual void InitGlobal(global_setting_type const& globalSettings) {
	}

	virtual void InitLocal(setting_type const& pipelineSettings) {
	}

	// called once per event before the pipelines are split
	virtual void ProduceGlobal(event_type const& event, product_type& product,
	                           global_setting_type const& globalSettings) const {
		LOG_FATAL("ProduceGlobal for producer " << this->GetProducerId() << " is not implemented");
	}

	// called once per event within a given pipeline
	virtual void ProduceLocal(event_type const& event, product_type & product, 
	                          setting_type const& pipelineSettings) const {
		LOG_FATAL("ProduceLocal for producer " << this->GetProducerId() << " is not implemented");
	}

	/// Must return a unique id of the producer.
	virtual std::string GetProducerId() const = 0;
};

