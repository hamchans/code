class AuthsController < ApplicationController
  def new
  end

  def create
    user = User.find_by(email: params[:auth][:email].downcase)
    if user && user.valid_password?(params[:auth][:password])
      session[:user_id] = user.id
      redirect_to root_path
    else
      flash.now[:notice] = "Invalid email/password combination"
#      redirect_to new_auth_path
      render :new
    end
  end

  def destroy
    session.delete(:user_id)
    @current_user = nil
    redirect_to root_path
  end
end
