#include "o3d3xx.h"
#include <cstdint>
#include <vector>
#include <opencv2/core/core.hpp>
#include <pcl/point_cloud.h>
#include "gtest/gtest.h"

// utility function to compare to opencv images
bool imgs_eq(cv::Mat a, cv::Mat b)
{
  return cv::countNonZero(a != b) == 0;
}

TEST(ImageBuffers_Tests, Cloud)
{
  o3d3xx::Camera::Ptr cam =
    o3d3xx::Camera::Ptr(new o3d3xx::Camera());

  o3d3xx::FrameGrabber::Ptr fg =
    o3d3xx::FrameGrabber::Ptr(new o3d3xx::FrameGrabber(cam));

  o3d3xx::ImageBuffer::Ptr img =
    o3d3xx::ImageBuffer::Ptr(new o3d3xx::ImageBuffer());

  EXPECT_TRUE(fg->WaitForFrame(img, 1000));

  pcl::PointCloud<o3d3xx::PointT>::Ptr cloud = img->Cloud();
  EXPECT_TRUE(cloud.use_count() == 2);

  int width = cloud->width;
  int height = cloud->height;

  // create a new scope
  {
    o3d3xx::ImageBuffer::Ptr img2 =
      o3d3xx::ImageBuffer::Ptr(new o3d3xx::ImageBuffer());

    EXPECT_TRUE(fg->WaitForFrame(img2, 1000));

    cloud = img2->Cloud();

    EXPECT_FALSE(img->Cloud() == cloud);
    EXPECT_TRUE(cloud.use_count() == 2);
  }

  EXPECT_TRUE(cloud.use_count() == 1);
  EXPECT_TRUE(cloud->width == width);
  EXPECT_TRUE(cloud->height == height);
}

TEST(ImageBuffers_Tests, DepthImage)
{
  o3d3xx::Camera::Ptr cam =
    o3d3xx::Camera::Ptr(new o3d3xx::Camera());

  o3d3xx::FrameGrabber::Ptr fg =
    o3d3xx::FrameGrabber::Ptr(new o3d3xx::FrameGrabber(cam));

  o3d3xx::ImageBuffer::Ptr img =
    o3d3xx::ImageBuffer::Ptr(new o3d3xx::ImageBuffer());

  EXPECT_TRUE(fg->WaitForFrame(img, 1000));

  cv::Mat dimg = img->DepthImage();
  cv::Mat dimg2 = img->DepthImage();
  EXPECT_TRUE(imgs_eq(dimg, dimg2));

  {
    o3d3xx::ImageBuffer::Ptr img2 =
      o3d3xx::ImageBuffer::Ptr(new o3d3xx::ImageBuffer());

    EXPECT_TRUE(fg->WaitForFrame(img2, 1000));

    dimg2 = img2->DepthImage();
    EXPECT_FALSE(imgs_eq(dimg, dimg2));
  }

  // img2 is now out of scope, we want to check that
  // dimg2 is still valid but not the same as dimg

  EXPECT_FALSE(imgs_eq(dimg, dimg2));
  EXPECT_EQ(dimg.size().height, dimg2.size().height);
  EXPECT_EQ(dimg.size().width, dimg2.size().width);
}

TEST(ImageBuffers_Tests, AmplitudeImage)
{
  o3d3xx::Camera::Ptr cam =
    o3d3xx::Camera::Ptr(new o3d3xx::Camera());

  o3d3xx::FrameGrabber::Ptr fg =
    o3d3xx::FrameGrabber::Ptr(new o3d3xx::FrameGrabber(cam));

  o3d3xx::ImageBuffer::Ptr img =
    o3d3xx::ImageBuffer::Ptr(new o3d3xx::ImageBuffer());

  EXPECT_TRUE(fg->WaitForFrame(img, 1000));

  cv::Mat aimg = img->AmplitudeImage();
  cv::Mat aimg2 = img->AmplitudeImage();
  EXPECT_TRUE(imgs_eq(aimg, aimg2));

  {
    o3d3xx::ImageBuffer::Ptr img2 =
      o3d3xx::ImageBuffer::Ptr(new o3d3xx::ImageBuffer());

    EXPECT_TRUE(fg->WaitForFrame(img2, 1000));

    aimg2 = img2->AmplitudeImage();
    EXPECT_FALSE(imgs_eq(aimg, aimg2));
  }

  // make sure aimg2 is still valid even though img2 is now out-of-scope

  EXPECT_FALSE(imgs_eq(aimg, aimg2));
  EXPECT_EQ(aimg.size().height, aimg2.size().height);
  EXPECT_EQ(aimg.size().width, aimg2.size().width);
}

TEST(ImageBuffers_Tests, ConfidenceImage)
{
  o3d3xx::Camera::Ptr cam =
    o3d3xx::Camera::Ptr(new o3d3xx::Camera());

  o3d3xx::FrameGrabber::Ptr fg =
    o3d3xx::FrameGrabber::Ptr(new o3d3xx::FrameGrabber(cam));

  o3d3xx::ImageBuffer::Ptr img =
    o3d3xx::ImageBuffer::Ptr(new o3d3xx::ImageBuffer());

  EXPECT_TRUE(fg->WaitForFrame(img, 1000));

  cv::Mat cimg = img->ConfidenceImage();
  cv::Mat cimg2 = img->ConfidenceImage();
  EXPECT_TRUE(imgs_eq(cimg, cimg2));

  {
    o3d3xx::ImageBuffer::Ptr img2 =
      o3d3xx::ImageBuffer::Ptr(new o3d3xx::ImageBuffer());

    EXPECT_TRUE(fg->WaitForFrame(img2, 1000));

    cimg2 = img2->ConfidenceImage();
    EXPECT_FALSE(imgs_eq(cimg, cimg2));
  }

  // make sure cimg2 is still valid even though img2 is now out-of-scope

  EXPECT_FALSE(imgs_eq(cimg, cimg2));
  EXPECT_EQ(cimg.size().height, cimg2.size().height);
  EXPECT_EQ(cimg.size().width, cimg2.size().width);
}

TEST(ImageBuffers_Tests, Bytes)
{
  o3d3xx::Camera::Ptr cam =
    o3d3xx::Camera::Ptr(new o3d3xx::Camera());

  o3d3xx::FrameGrabber::Ptr fg =
    o3d3xx::FrameGrabber::Ptr(new o3d3xx::FrameGrabber(cam));

  o3d3xx::ImageBuffer::Ptr img =
    o3d3xx::ImageBuffer::Ptr(new o3d3xx::ImageBuffer());

  EXPECT_TRUE(fg->WaitForFrame(img, 1000));

  std::vector<std::uint8_t> b1 = img->Bytes();
  std::vector<std::uint8_t> b2 = img->Bytes();

  EXPECT_EQ(b1, b2);
  EXPECT_NO_THROW(b1[0]+=1);
  EXPECT_NE(b1, b2);

  std::vector<std::uint8_t> b3;
  EXPECT_EQ(0, b3.size());
  {
    o3d3xx::ImageBuffer::Ptr img2 =
      o3d3xx::ImageBuffer::Ptr(new o3d3xx::ImageBuffer());

    EXPECT_TRUE(fg->WaitForFrame(img2, 1000));

    b3 = img2->Bytes();
  }

  EXPECT_NE(0, b3.size());
  EXPECT_NE(b1, b3);

  img->SetBytes(b3);
  b1 = img->Bytes();
  EXPECT_NE(b1, b3);

  img->SetBytes(b3, true); // set copy flag
  b1 = img->Bytes();
  EXPECT_EQ(b1, b3);
}
