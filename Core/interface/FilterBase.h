/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include <boost/noncopyable.hpp>

#include <map>
#include <sstream>

// forward define to be able to use the event pipeline here
template<class TTypes>
class Pipeline;

template<class TTypes>
class FilterBase: public boost::noncopyable {
public:

	typedef typename TTypes::event_type event_type;
	typedef typename TTypes::product_type product_type;
	typedef typename TTypes::setting_type setting_type;
	typedef typename TTypes::global_setting_type global_setting_type;

	virtual ~FilterBase() {
	}

	// initialise global pre-filters
	virtual void InitGlobal(global_setting_type const& globalSettings) {
	}

	// initialise local (pre-) filters
	virtual void InitLocal(setting_type const& pipelineSettings) {
	}

	virtual void Finish() {
	}

	/*
	 * Must return a unique id of the filter.
	 */
	virtual std::string GetFilterId() const = 0;

	// process global event
	virtual bool DoesEventPassGlobal(event_type const& event,
			product_type const& product, global_setting_type const& globalSettings) const
	{
		return true;
	}

	// process local event
	virtual bool DoesEventPassLocal(event_type const& event,
			product_type const& product, setting_type const& settings) const
	{
		return true;
	}

	virtual std::string ToString(bool bVerbose = false) {
		return GetFilterId();
	}

};
