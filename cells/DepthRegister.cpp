#include <ecto/ecto.hpp>
#include <image_pipeline/stereo_camera_model.h>

using ecto::tendrils;

namespace image_pipeline
{
  struct DepthRegister
  {
    static void
    declare_params(tendrils& params)
    {
      params.declare(&DepthRegister::metric_, "metric","A metric scalar.",0.001);
    }
    static void
    declare_io(const tendrils& params, tendrils& in, tendrils& out)
    {
      in.declare<StereoCameraModel>("rgbd_model", "Camera model.").required(true);
      in.declare<cv::Mat>("image", "The input image.").required(true);
      out.declare<cv::Mat>("image", "The registered image.");
    }
    void
    configure(const tendrils& params, const tendrils& in, const tendrils& out)
    {
      rgbd_camera = in["rgbd_model"]; 
      image_in = in["image"];
      image_out = out["image"];
    }
    int process(const tendrils& /*in*/, const tendrils& /*out*/)
    {
      cv::Mat output;
      rgbd_camera->registerDepthImage(*image_in,output,*metric_);

      *image_out = output;
      return ecto::OK;
    }
    ecto::spore<StereoCameraModel> rgbd_camera;
    ecto::spore<cv::Mat> image_in, image_out;
    ecto::spore<double> metric_;
  };
}

ECTO_CELL(image_pipeline, image_pipeline::DepthRegister, "DepthRegister",
          "Given a depth image and rgbd image model, register the depth to the image.");
