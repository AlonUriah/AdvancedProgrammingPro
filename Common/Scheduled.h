/*
 * The Scheduled interface. Each class
 * which implements this interface, is
 * obligated to time, and manages its
 * actions by the Clock class, which is
 * the publisher.
 */
#ifndef SCHEDULED_H_
#define SCHEDULED_H_

class Scheduled {
public:
	/*
	 * Scheduled constructor
	 */
	Scheduled();
	/*
	 * Scheduled destructor
	 */
	virtual ~Scheduled();
	/*
	 * When time passed by 1,
	 * each scheduled object should respond
	 * and do something according to the
	 * time currency.
	 */
	virtual void timePassed(int time) = 0;
};

#endif /* SCHEDULED_H_ */