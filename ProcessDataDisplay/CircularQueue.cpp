class CircularQueue {
private:
	int front, rear;
	int nums[];

public:
	 CircularQueue(int initialSize) {
		this->front = this->rear = -1;
		this->nums = new int[initialSize];
	}
};