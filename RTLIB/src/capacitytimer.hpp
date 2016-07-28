#ifndef __CAPACITY_TIMER_HPP__
#define __CAPACITY_TIMER_HPP__

#include <simul.hpp>
#include <entity.hpp>

namespace RTSim {
    using namespace MetaSim;

    class CapacityTimer : public Entity {
    public:
        typedef enum {RUNNING, STOPPED} status_t;

        CapacityTimer();
        ~CapacityTimer();

        void start(double speed=1.0);
        double stop();
        status_t get_status() { return status; }
        double get_value() const;
        void set_value(const double &v);

	/** 
	    Returns how much time from now it will take for this timer
	    to reach value v.
	 */
        Tick get_intercept(const Tick &v) const;

        void newRun();
        void endRun();
    private:
        Tick last_time;
        //Tick value;
	double value;
        status_t status;
        double der;
    };
}

#endif
