#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <opencv2/opencv.hpp>

int main() {
    // Load 3D point cloud data
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    if (pcl::io::loadPCDFile<pcl::PointXYZ>("/home/ubutnu/catkin_ws/src/pcl_tutorial/materials/cornerMap.pcd", *cloud) == -1) {
        PCL_ERROR("Couldn't read input PCD file.\n");
        return (-1);
    }

    // Define camera parameters for the top view
    double focal_length_x = 525.0; // Adjust as needed
    double focal_length_y = 525.0; // Adjust as needed
    double camera_height = 50.0;   // Height of the camera from the ground

    // Create a 2D image to represent the projection (top view)
    int width = 1280;  // Width of the 2D image
    int height = 960; // Height of the 2D image
    cv::Mat projection_image(height, width, CV_8UC3, cv::Scalar(0, 0, 0)); // Create a black image

    // Project 3D points to 2D image (top view)
    for (const auto& point : cloud->points) {
        double x = point.x;
        double y = point.y;
        double z = point.z - camera_height; // Adjust for camera height

        int u = static_cast<int>(focal_length_x * x / z + width / 2.0);
        int v = static_cast<int>(focal_length_y * y / z + height / 2.0);

        // Check if the point is within the image bounds
        if (u >= 0 && u < width && v >= 0 && v < height) {
            cv::circle(projection_image, cv::Point(u, v), 2, cv::Scalar(0, 0, 255), -1); // Draw a red circle
        }
    }

    // Display the 2D projection
    cv::imshow("2D Projection (Top View)", projection_image);
    cv::waitKey(0);

    return 0;
}
