/* Copyright (c) 2013 - All Rights Reserved
 *   Thomas Hauth  <Thomas.Hauth@cern.ch>
 *   Joram Berger  <Joram.Berger@cern.ch>
 *   Dominik Haitz <Dominik.Haitz@kit.edu>
 */

#pragma once

#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_list.hpp>

#include "Pipeline.h"
#include "EventProviderBase.h"
#include "ProgressReport.h"

/**
   \brief Class to manage all registered Pipelines and to connect them to the event.
   
   The PipelineRunner utilizes a user-provided EventProvider to load events and passes them to all 
   registered Pipelines. The EventProvider is passed to the function PipelineRunner::RunPipelines 
   as an argument. Furthermore, GlobalProducers can be registered, which can generate Pipeline-
   independet products of the event. These GlobalProducers are run before any pipeline is started 
   and the generated data is passed on to the pipelines.
*/
template<typename TPipeline, typename TTypes>

class PipelineRunner: public boost::noncopyable {
public:

	PipelineRunner() {
		// this is the default
		// use AddProgressReport / ClearProgressReports to adapt it to your needs
		AddProgressReport(new ConsoleProgressReport());
	}

	typedef TPipeline pipeline_type;

	typedef typename TTypes::event_type event_type;
	typedef typename TTypes::product_type product_type;
	typedef typename TTypes::setting_type setting_type;
	typedef typename TTypes::global_setting_type global_setting_type;

	typedef ProducerBase< TTypes > producer_base_type;
	typedef FilterBase< TTypes > filter_base_type;
	typedef ConsumerBase< TTypes > consumer_base_type;


	typedef boost::ptr_list<TPipeline> Pipelines;
	typedef typename Pipelines::iterator PipelinesIterator;

	typedef boost::ptr_list< filter_base_type > Filters;
	typedef typename Filters::iterator FiltersIterator;

	typedef boost::ptr_list< producer_base_type > Producer;
	typedef typename Producer::iterator ProducerIterator;

	typedef boost::ptr_list<ProgressReportBase> ProgressReportList;
	typedef typename ProgressReportList::iterator ProgressReportIterator;

	/// Add a pipeline. The object is destroy in the destructor of the PipelineRunner.
	void AddPipeline(TPipeline* pline) {
		m_pipelines.push_back(pline);
	}

	/// Add a global pre-filter. The object is destroy in the destructor of the PipelineRunner.
	void AddGlobalPreFilter(filter_base_type* filter) {
		m_globalPreFilters.push_back(filter);
	}

	/// Add a GlobalProducer. The object is destroy in the destructor of the PipelineRunner.
	void AddGlobalProducer(producer_base_type* prod) {
		m_globalProducer.push_back(prod);
	}

	/// Add a range of pipelines. The object is destroy in the destructor of the PipelineRunner.
	void AddPipelines(std::vector<TPipeline*> pVec) {

		for (auto * it : pVec) {
			AddPipeline(it);
		}
	}

	/// Run the GlobalProducers and all pipelines. Give any pipeline setting here: only the 
	/// global producer will read from the global settings ...
	template<class TEventProvider>
	void RunPipelines(
			/*EventProviderBase<TTypes>*/TEventProvider & evtProvider,
			global_setting_type const& globalSettings) {
		long long firstEvent = 0; // settings.Global()->GetSkipEvents();
		long long nEvents = evtProvider.GetEntries();
		/*if (settings.Global()->GetEventCount() >= 0)
		 nEvents = firstEvent + settings.Global()->GetEventCount();
		 if (firstEvent != 0 || nEvents != evtProvider.GetEntries())
		 LOG(red << "Warning: Custom range of events: " << firstEvent << " to " << nEvents << reset)
		 */

		// init global pre-filters
		for (FiltersIterator it = m_globalPreFilters.begin();
				it != m_globalPreFilters.end(); it++) {
			it->InitGlobal(globalSettings);
		}

		// init global producers
		for (ProducerIterator it = m_globalProducer.begin();
				it != m_globalProducer.end(); it++) {
			it->InitGlobal(globalSettings);
		}

		for (long long i = firstEvent; i < nEvents; ++i) {

			for (ProgressReportIterator it = m_progressReport.begin();
					it != m_progressReport.end(); it++) {
				it->update(i, nEvents);
			}
			if (!evtProvider.GetEntry(i))
				break;

			product_type productGlobal;
			FilterResult globalFilterResult;

			// call global pre-filters
			for (FiltersIterator it = m_globalPreFilters.begin();
					it != m_globalPreFilters.end(); it++) {
				
				globalFilterResult.SetFilterDecisions(it->GetFilterId(),
						it->DoesEventPassGlobal(evtProvider.GetCurrentEvent(),
								productGlobal, globalSettings));
			}

			// call global products
			for (ProducerIterator it = m_globalProducer.begin();
					it != m_globalProducer.end(); it++) {
				it->ProduceGlobal(evtProvider.GetCurrentEvent(),
						productGlobal, globalSettings);
			}

			// run the pipelines
			for (PipelinesIterator it = m_pipelines.begin();
					it != m_pipelines.end(); it++) {
				if (it->GetSettings().GetLevel() == 1)
					it->RunEvent(evtProvider.GetCurrentEvent(),
							productGlobal, globalFilterResult);
			}
		}

		for (ProgressReportIterator it = m_progressReport.begin();
				it != m_progressReport.end(); it++) {
			it->finish(nEvents - 1, nEvents);
		}

		// first safe the results ( > plots ) from all level one pipelines
		for (PipelinesIterator it = m_pipelines.begin();
				!(it == m_pipelines.end()); it++) {
			if (it->GetSettings().GetLevel() == 1)
				it->FinishPipeline();
		}

		// run the pipelines greater level one
		bool noPipelineRun = false;
		size_t curLevel = 2;
		while (!noPipelineRun) {
			noPipelineRun = true;
			for (PipelinesIterator it = m_pipelines.begin();
					it != m_pipelines.end(); it++) {
				if (it->GetSettings().GetLevel() == curLevel) {
					noPipelineRun = false;

					it->Run();
					it->FinishPipeline();
				}
			}
			curLevel++;
		}
	}

	void AddProgressReport(ProgressReportBase * p) {
		m_progressReport.push_back(p);
	}

	void ClearProgressReports() {
		m_progressReport.clear();
	}

private:

	Pipelines m_pipelines;
	Filters m_globalPreFilters;
	Producer m_globalProducer;
	ProgressReportList m_progressReport;
};

