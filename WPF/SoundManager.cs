using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;

namespace WPF
{
    class SoundManager
    {
        static SoundManager instance = null;
        public static SoundManager Instance()
        {
            if( instance == null )
                instance = new SoundManager();
            return instance;
        }

        enum Mode {
            MENUS,
            INGAME, 
            NONE
        } 

        MediaPlayer tetradFound = new MediaPlayer();
        MediaPlayer inGame = new MediaPlayer();
        MediaPlayer menus = new MediaPlayer();
        MediaPlayer currentMedia = null;
        bool m_muted = false;
        SoundManager()
        {
            tetradFound.Open(new Uri("Resources/tetrad.mp3", UriKind.Relative));
            inGame.Open(new Uri("Resources/ingame.mp3", UriKind.Relative));
            menus.Open(new Uri("Resources/m1.mp3", UriKind.Relative));
        }

        void OnMediaEnded(object sender, EventArgs e)
        {
            if( ! m_muted )
            {
                currentMedia.Stop();
                currentMedia.Play();
            }
        }

        public bool Muted
        {
            get
            {
                return m_muted;
            }
            set {
                m_muted = value;
                if (currentMedia != null)
                {
                    if (m_muted == true)
                    {
                        currentMedia.Pause();
                    }
                    else
                    {
                        currentMedia.Play();
                    }
                }
            }
        }

        void SwitchMusic(MediaPlayer cur)
        {
            if( currentMedia != null )
                currentMedia.Stop();
            currentMedia = cur;
            currentMedia.Stop();
            if (!m_muted)
                currentMedia.Play();
            currentMedia.MediaEnded += new EventHandler(OnMediaEnded);
        }

        public void PlayGameMusic()
        {
            SwitchMusic(inGame);
        }

        public void PlayMenuMusic()
        {
            SwitchMusic(menus);
        }

        
        public void OnTetradFound(object sender, TetradFoundEventArgs e)
        {
            try
            {
                if (e.Player.Team == ObjectModel.PlayerTeam.Home)
                {
                    if( ! m_muted )
                    {
                        tetradFound.Stop();
                        tetradFound.Play();
                    }
                }
            }
            catch (Exception ex)
            {
                Logger.Log(ex.Message);
            }   
        }

        public void OnGameOver(object sender, GameOverEventArgs e)
        {
            
        }

    }
}
