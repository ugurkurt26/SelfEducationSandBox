/*
 * Object.hpp
 *
 *  Created on: Aug 11, 2023
 *      Author: ugur
 */

#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

class Object
{
	public:
		Object();
		~Object();

		void LoadTexturesFromPath(std::string texturesPathasString, bool isFlip);
		void ChangeAnimation(cv::Point directionToLook);
		std::shared_ptr<cv::Mat> GetCharacterImage();

		virtual void UpdateNewPosition(cv::Point newPositionCoordinate, cv::Size fieldBoundaries);
		void UpdateSpeed(int newSpeed);

		virtual void collisionEffects() = 0;

		std::string objectName;
		cv::Size size;
		cv::Point position;
		int speed;

	private:
		boost::filesystem::path texturesPath;

		std::vector<cv::Mat> textures;
		std::pair<int, std::shared_ptr<cv::Mat>> selectedTexture;
};

#endif /* OBJECT_HPP_ */
