class EntriesController < ApplicationController
  before_action :set_entry, only: %i[ destroy ]

  def new
    @entry = Entry.new(room_id: params[:room_id])
  end

  def create
    @entry = Entry.new(entry_params)    
    respond_to do |format|
      if @entry.save
#        NoticeMailer.register(@entry).deliver_now
        format.html { redirect_to room_path(@entry.room), notice: "Entry was successfully created." }
        format.json { render :show, status: :created, location: @entry }
      else
        format.html { render :new, status: :unprocessable_entity }
        format.json { render json: @entry.errors, status: :unprocessable_entity }
      end
    end
  end

  def destroy
    @entry.destroy
    respond_to do |format|
      format.html { redirect_to room_path(@entry.room), notice: "Entry was successfully destroyed." }
      format.json { head :no_content }
    end
  end

  def index
    @entries = Entry.all
  end

  def confirm
    @entry = Entry.new(entry_params)
    if @entry.invalid?
      render :new
    end
  end

  def confirm_back
    @entry = Entry.new(entry_params)
    render :new
  end

  private
  def entry_params
    params.require(:entry).permit(:user_name, :user_email, :reserved_date, :usage_time, :room_id, :people)
  end
  
  def set_entry
    @entry = Entry.find(params[:id])
  end

end
