#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <opencv2/opencv.hpp>

int main() {
    // Load 3D point cloud data
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    if (pcl::io::loadPCDFile<pcl::PointXYZ>("/home/ubutnu/catkin_ws/visiontower_cornerpcd/inliers.pcd", *cloud) == -1) {
        PCL_ERROR("Couldn't read input PCD file.\n");
        return (-1);
    }

    // Create a 2D image to represent the projection (2D map)
    int width = 1280;  // Width of the 2D image
    int height = 960; // Height of the 2D image
    cv::Mat projection_image(height, width, CV_8UC3, cv::Scalar(255, 255, 255)); // Create a white image

    // Define transformation parameters (you may need to adjust these based on your setup)
    double scale_factor = 10.0; // Adjust the scaling factor as needed

    // Project 3D points to 2D image (ignore Z axis)
    for (const auto& point : cloud->points) {
        double x = point.x;
        double z = point.z;

        int u = static_cast<int>(x * scale_factor + width / 2.0);
        int v = static_cast<int>(z * scale_factor + height / 2.0);

        // Check if the point is within the image bounds
        if (u >= 0 && u < width && v >= 0 && v < height) {
            projection_image.at<cv::Vec3b>(v, u) = cv::Vec3b(0, 0, 0); // Set the pixel to black
        }
    }

    // Display the 2D projection (2D map)
    cv::imshow("2D Map", projection_image);

    // Save the 2D image to a file
    cv::imwrite("/home/ubutnu/catkin_ws/visiontower_cornerpcd/2dimage.png", projection_image);


    cv::waitKey(0);

    return 0;
}
