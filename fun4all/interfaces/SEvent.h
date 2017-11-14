/*
 * SEvent.h
 *
 *  Created on: Oct 29, 2017
 *      Author: yuhw
 */

#ifndef _H_SEvent_H_
#define _H_SEvent_H_

#include "SHitMap.h"

#include <phool/PHObject.h>

#include <map>
#include <iostream>

class SEvent: public PHObject {

public:

	enum TriggerMask {
		NIM1 = 0,
		NIM2 = 1,
		NIM3 = 2,
		NIM4 = 3,
		NIM5 = 4,
		MATRIX1 = 5,
		MATRIX2 = 6,
		MATRIX3 = 7,
		MATRIX4 = 8,
		MATRIX5 = 9
	};

	virtual ~SEvent() {}

	void Reset() {}

	virtual void identify(std::ostream& os = std::cout) const {
		std::cout
		<< "---SEvent::identify: abstract base-------------------"
		<< std::endl;
	}

	virtual int get_run_id() const = 0;
	virtual void set_run_id(const int a) = 0;

	virtual int get_spill_id() const = 0;
	virtual void set_spill_id(const int a) = 0;

	virtual int get_event_id() const = 0;
	virtual void set_event_id(const int a) = 0;

	virtual int get_coda_event_id() const = 0;
	virtual void set_coda_event_id(const int a) = 0;


	virtual int get_data_quality() const = 0;
	virtual void set_data_quality(const int a) = 0;

	virtual int get_vme_time() const = 0;
	virtual void set_vme_time(const int a) = 0;

	virtual bool get_trigger(const SEvent::TriggerMask i) const = 0;
	virtual void set_trigger(const SEvent::TriggerMask i, const bool a) = 0;

	virtual int get_raw_matrix(const unsigned short i) const = 0;
	virtual void set_raw_matrix(const unsigned short i, const bool a) = 0;

	virtual int get_after_inh_matrix(const unsigned short i) const = 0;
	virtual void set_after_inh_matrix(const unsigned short i, const bool a) = 0;



protected:

	SEvent(){};

ClassDef(SEvent, 1);
};

#endif /* _H_SEvent_H_ */
